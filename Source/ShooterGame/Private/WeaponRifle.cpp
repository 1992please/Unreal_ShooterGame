// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "WeaponRifle.h"

AWeaponRifle::AWeaponRifle()
{
	WeaponType = EWeaponType::Rifle;
	AttachSocketNameFPP = "S_Rifle";

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponObject(TEXT("SkeletalMesh'/Game/Weapons/Rifle/Rifle.Rifle'"));
	if (WeaponObject.Succeeded())
		WeaponMesh->SetSkeletalMesh(WeaponObject.Object);
}

void AWeaponRifle::Fire()
{
	Super::Fire();


}

void AWeaponRifle::Reload()
{
	Super::Reload();

}