// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "WeaponShotGun.h"

AWeaponShotGun::AWeaponShotGun()
{
	WeaponType = EWeaponType::Shotgun;
	AttachSocketNameFPP = "S_ShotGun";

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponObject(TEXT("SkeletalMesh'/Game/Weapons/ShotGun/ShotGun.ShotGun'"));
	if(WeaponObject.Succeeded())
		WeaponMesh->SetSkeletalMesh(WeaponObject.Object);
}



