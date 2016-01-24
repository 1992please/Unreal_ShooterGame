// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Weapon.h"


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

	// intialize Attributes
	MaxAmmoInMag = 6;
	MaxAmmoInBackpack = 30;
	ReloadTime = 2;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	// Start with Full Ammo
	CurrentAmmoInMag = MaxAmmoInMag;
	CurrentAmmoInBackpack = MaxAmmoInBackpack;
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWeapon::Fire()
{
	CurrentAmmoInMag--;
}

void AWeapon::Reload()
{
	int32 NeededAmmo = MaxAmmoInMag - CurrentAmmoInMag;
	// Check to see the CurrentAmmoInBackpack is enough for a full reload
	if (CurrentAmmoInBackpack > NeededAmmo)
	{
		// full reload
		CurrentAmmoInBackpack -= NeededAmmo;
		CurrentAmmoInMag = MaxAmmoInMag;
	}
	else
	{
		CurrentAmmoInMag += CurrentAmmoInBackpack;
		CurrentAmmoInBackpack = 0;
	}
}

bool AWeapon::HaveAmmoInMag()
{
	return CurrentAmmoInMag > 0;
}

bool AWeapon::IsMagFull()
{
	return CurrentAmmoInMag == 0;
}

bool AWeapon::HaveAmmoInBackpack()
{
	return CurrentAmmoInBackpack > 0;
}