// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Weapon.h"
#include "GamePlayCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
		bPendingReload = true;
		DetermineWeaponState();
	}

	float AnimDuration = 0;
	if (ReloadAnim)
	{
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

void AWeapon::ReloadWeapon()
{
	int32 ClipDelta = FMath::Min(WeaponConfig.AmmoPerClip - CurrentAmmoInClip, CurrentAmmo - CurrentAmmoInClip);
	if (WeaponConfig.bInfiniteClip)
	{
		ClipDelta = WeaponConfig.AmmoPerClip - CurrentAmmoInClip;
	}

	if (ClipDelta > 0)
	{
		CurrentAmmoInClip += ClipDelta ;
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
	if (NewState == EWeaponState::Firing && PrevState != EWeaponState::Firing)
	{
		OnBurstStarted();
	}

	CurrentState = NewState;

	if (NewState != EWeaponState::Firing && PrevState == EWeaponState::Firing)
	{
		OnBurstFinished();
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
}

void AWeapon::HandleFiring()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Fire");
	if (CurrentAmmoInClip > 0 || WeaponConfig.bInfiniteClip || WeaponConfig.bInfiniteAmmo && CanFire())
	{
		SimulateWeaponFire();

		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			//FireWeapon();

			UseAmmo();

			// update firing FX on remote clients if function was called on server
			//BurstCounter++;
		}
	}
	else if (CanReload())
	{
		StartReload();
	}
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void AWeapon::SimulateWeaponFire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "simulate Fire");
	// Particle Effects
	if (MuzzleFX)
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
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Sound");
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
	return (CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing) && bGotAmmo;
}

bool AWeapon::CanFire() const
{
	return (CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing);
}