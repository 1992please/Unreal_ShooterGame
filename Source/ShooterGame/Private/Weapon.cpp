// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Weapon.h"
#include "MyStaticLibrary.h"
#include "ShooterDamageType.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	WeaponMesh->AttachTo(RootComponent);

	AttachSocketNameFPP = "WeaponPoint";
	AttachSocketNameTPP = "WeaponPoint";
	WeaponType = EWeaponType::Rifle;
	bWantsToFire = false;
	bPendingReload = false;
	CurrentState = EWeaponState::Idle;
	LastFireTime = 0;
	bPlayingFireAnim = false;
	SpreadDecreaseSpeed = 0.1f;
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (WeaponConfig.InitialClips > 0)
	{
		CurrentAmmoInClip = WeaponConfig.AmmoPerClip;
		CurrentAmmo = WeaponConfig.AmmoPerClip * WeaponConfig.InitialClips;
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Always decrease the spread to the SpreadMin
	if (SpreadCurrent > SpreadMin)
	{
		DecreaseSpread(DeltaTime * SpreadDecreaseSpeed);
	}
}

void AWeapon::StartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void AWeapon::StopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

void AWeapon::StartReload()
{
	if (CanReload())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Start Reload");
		bPendingReload = true;
		DetermineWeaponState();

		float AnimDuration = 0;
		if (ReloadAnim)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Play Reload Animation");
			AnimDuration = MyPawn->PlayAnimMontage(ReloadAnim);
		}

		if (AnimDuration <= 0.0f)
		{
			AnimDuration = WeaponConfig.NoAnimReloadDuration;
		}

		GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &AWeapon::StopReload, AnimDuration, false);
		if (Role == ROLE_Authority)
		{
			ReloadWeapon();
		}

		if (ReloadSound)
		{
			UGameplayStatics::SpawnSoundAttached(ReloadSound, MyPawn->GetRootComponent());
		}
	}
}

void AWeapon::ReloadWeapon()
{
	int32 ClipDelta = FMath::Min(WeaponConfig.AmmoPerClip - CurrentAmmoInClip, CurrentAmmo - CurrentAmmoInClip);
	if (WeaponConfig.bInfiniteClip)
	{
		ClipDelta = WeaponConfig.AmmoPerClip - CurrentAmmoInClip;
	}

	if (ClipDelta > 0)
	{
		CurrentAmmoInClip += ClipDelta;
	}

	if (WeaponConfig.bInfiniteClip)
	{
		CurrentAmmo = FMath::Max(CurrentAmmoInClip, CurrentAmmo);
	}
}

void AWeapon::StopReload()
{
	if (CurrentState == EWeaponState::Reloading)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "stop Reload Animation");
		bPendingReload = false;
		DetermineWeaponState();
		if (ReloadAnim)
		{
			MyPawn->StopAnimMontage(ReloadAnim);
		}
	}
}

void AWeapon::SetOwningPawn(AGamePlayCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
		// net owner for RPC calls
		SetOwner(NewOwner);
	}
}

void AWeapon::DetermineWeaponState()
{
	EWeaponState::Type NewState = EWeaponState::Idle;
	if (bPendingReload)
	{
		if (!CanReload())
		{
			NewState = CurrentState;
		}
		else
		{
			NewState = EWeaponState::Reloading;
		}
	}
	else if (bWantsToFire && CanFire())
	{
		NewState = EWeaponState::Firing;
	}
	SetWeaponState(NewState);
}

void AWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	const EWeaponState::Type PrevState = CurrentState;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, NewState ==  EWeaponState::Idle ?"Set State Idle": "Set State fire");
	if (NewState != EWeaponState::Firing && PrevState == EWeaponState::Firing)
	{
		OnBurstFinished();
	}

	CurrentState = NewState;

	if (NewState == EWeaponState::Firing && PrevState != EWeaponState::Firing)
	{
		OnBurstStarted();
	}


}

void AWeapon::OnBurstStarted()
{
	const float GameTime = GetWorld()->GetTimeSeconds();
	if (LastFireTime > 0 && WeaponConfig.TimeBetweenShots > 0.0f && LastFireTime + WeaponConfig.TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AWeapon::HandleFiring, LastFireTime + WeaponConfig.TimeBetweenShots - GameTime, false);
	}
	else
	{
		HandleFiring();
	}

}

void AWeapon::OnBurstFinished()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Burst Finished");
	if (MuzzlePSC != NULL)
	{
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = NULL;
	}

	if (bPlayingFireAnim && bLoopedFireAnim)
	{
		MyPawn->StopAnimMontage(PawnAnim);
		bPlayingFireAnim = false;
	}

	if (FireAC)
	{
		FireAC->FadeOut(.1, 0);
		FireAC = NULL;
		if (FireFinishSound)
			UGameplayStatics::SpawnSoundAttached(FireFinishSound, MyPawn->GetRootComponent());
	}

	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);

	bRefiring = false;
}

void AWeapon::HandleFiring()
{


	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Fire");
	if (CurrentAmmoInClip > 0 || WeaponConfig.bInfiniteClip || WeaponConfig.bInfiniteAmmo && CanFire())
	{
		SimulateWeaponFire();

		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			FireWeapon();

			UseAmmo();

			// update firing FX on remote clients if function was called on server
			//BurstCounter++;
		}
	}
	else if (CanReload())
	{
		StartReload();
	}
	else if (bRefiring)
	{
		OnBurstFinished();
	}

	LastFireTime = GetWorld()->GetTimeSeconds();

	if (CurrentAmmoInClip <= 0 && CanReload())
	{
		StartReload();
	}

	bRefiring = (CurrentState == EWeaponState::Firing && WeaponConfig.TimeBetweenShots > 0);

	if (bRefiring)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AWeapon::HandleFiring, WeaponConfig.TimeBetweenShots, false);
	}
}

void AWeapon::SimulateWeaponFire()
{
	// Particle Effects
	if (MuzzleFX && (!bLoopedMuzzleFX || MuzzlePSC == NULL))
	{
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, WeaponMesh, MuzzleAttachPoint);
	}

	// Hand Animation
	if (!bPlayingFireAnim || !bLoopedFireAnim)
	{
		MyPawn->PlayAnimMontage(PawnAnim);
		bPlayingFireAnim = true;
	}

	// Sound Effects
	if (bLoopedFireSound && FireAC == NULL && FireLoopSound)
	{
		FireAC = UGameplayStatics::SpawnSoundAttached(FireLoopSound, MyPawn->GetRootComponent());
	}
}

void AWeapon::UseAmmo()
{
	if (!WeaponConfig.bInfiniteAmmo)
	{
		CurrentAmmoInClip--;
		if (!WeaponConfig.bInfiniteClip)
		{
			CurrentAmmo--;
		}
	}
}

bool AWeapon::CanReload() const
{
	bool bGotAmmo = CurrentAmmoInClip < WeaponConfig.AmmoPerClip && (CurrentAmmo - CurrentAmmoInClip > 0 || WeaponConfig.bInfiniteAmmo);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("bGorAmmo: %d"), CurrentAmmo - CurrentAmmoInClip > 0));
	return (CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing) && bGotAmmo;
}

bool AWeapon::CanFire() const
{
	return (CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing);
}

void AWeapon::CalculateShootInformations(UCameraComponent* Camera, USceneComponent* WeaponMesh, FName WeaponFireSocketName, FTransform& ProjectileTransform, FHitResult& HitResult, FVector& StartLocation)
{
	// the End of the line trance by making some calculation of the forward vector of the camera taking into consideration the the current spread
	//SpreadCurrent = FMath::RandRange(SpreadMin, SpreadMax);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, FString::Printf(TEXT("SpreadCurrent: %.2f"), SpreadCurrent));
	const FVector LocalEndPoint = FMath::VRandCone(Camera->GetForwardVector(), SpreadCurrent) * 10000 + Camera->GetComponentLocation();
	const FVector WeaponFireSocketLocation = WeaponMesh->GetSocketLocation(WeaponFireSocketName);


	FHitResult Impact = WeaponTrace(Camera->GetComponentLocation(), LocalEndPoint);
	//World->DebugDrawTraceTag = TraceTag;
	if (Impact.bBlockingHit)
	{
		ProjectileTransform = FTransform((Impact.ImpactPoint - WeaponFireSocketLocation).Rotation(), WeaponFireSocketLocation, FVector(1, 1, 1));
	}
	else
	{
		ProjectileTransform = FTransform(Camera->GetComponentRotation(), WeaponFireSocketLocation, FVector(1, 1, 1));
		Impact.ImpactPoint = LocalEndPoint;
	}
	HitResult = Impact;

}

void AWeapon::IncreaseSpread(float IncreaseAmount)
{
	const float SpreadNew = SpreadCurrent + IncreaseAmount;
	if (SpreadNew >= SpreadMax)
	{
		SpreadCurrent = SpreadMax;
	}
	else
	{
		SpreadCurrent = SpreadNew;
	}
}

void AWeapon::DecreaseSpread(float DecreaseAmount)
{
	const float SpreadNew = SpreadCurrent - DecreaseAmount;
	if (SpreadNew <= SpreadMin)
	{
		SpreadCurrent = SpreadMin;
	}
	else
	{
		SpreadCurrent = SpreadNew;
	}
}

void AWeapon::AddDamageTo(FHitResult& HitResult, FVector& StartLocation)
{
	if (!DamageType)
	{
		return;
	}
	float ActualHitDamage = HitDamage;

	/* Handle special damage location on the zombie body (types are setup in the Physics Asset of the zombie */
	UShooterDamageType* DmgType = Cast<UShooterDamageType>(DamageType->GetDefaultObject());
	UPhysicalMaterial * PhysMat = HitResult.PhysMaterial.Get();
	if (PhysMat && DmgType)
	{
		if (PhysMat->SurfaceType == SURFACE_HEAD)
		{
			ActualHitDamage *= DmgType->GetHeadDamageModifier();
		}
		else if (PhysMat->SurfaceType == SURFACE_LIMB)
		{
			ActualHitDamage *= DmgType->GetLimbDamageModifier();
		}
	}

	FPointDamageEvent PointDmg;
	PointDmg.DamageTypeClass = DamageType;
	PointDmg.HitInfo = HitResult;
	PointDmg.ShotDirection = (StartLocation - HitResult.ImpactPoint).GetSafeNormal();
	PointDmg.Damage = ActualHitDamage;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, PointDmg.ShotDirection.ToString());
	AActor *Actor = HitResult.GetActor();
	if (Actor)
	{
		Actor->TakeDamage(PointDmg.Damage, PointDmg, MyPawn->Controller, this);
	}
}

FHitResult AWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}