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
	//int32 NeededAmmo = MaxAmmoInMag - CurrentAmmoInMag;
	//// Check to see the CurrentAmmoInBackpack is enough for a full reload
	//if (CurrentAmmoInBackpack > NeededAmmo)
	//{
	//	// full reload
	//	CurrentAmmoInBackpack -= NeededAmmo;
	//	CurrentAmmoInMag = MaxAmmoInMag;
	//}
	//else
	//{
	//	CurrentAmmoInMag += CurrentAmmoInBackpack;
	//	CurrentAmmoInBackpack = 0;
	//}
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
	if (bWantsToFire && CanFire())
	{
		NewState = EWeaponState::Firing;
	}
	SetWeaponState(NewState);
}

void AWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	if (NewState == EWeaponState::Firing && CurrentState != EWeaponState::Firing)
	{
		OnBurstStarted();
	}

	if (NewState != EWeaponState::Firing && CurrentState != EWeaponState::Firing)
	{
		OnBurstFinished();
	}
	CurrentState = NewState;
}

void AWeapon::OnBurstStarted()
{
	const float GameTime = GetWorld()->GetTimeSeconds();
	if (LastFireTime > 0 && WeaponConfig.TimeBetweenShots > 0.0f && LastFireTime + WeaponConfig.TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AWeapon::HandleFiring, LastFireTime + WeaponConfig.TimeBetweenShots, false);
	}
	else
	{
		HandleFiring();
	}

}

void AWeapon::OnBurstFinished()
{
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
	return 0;
}

bool AWeapon::CanFire() const
{
	return (CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing);
}