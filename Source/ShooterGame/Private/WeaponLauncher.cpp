// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "WeaponLauncher.h"



AWeaponLauncher::AWeaponLauncher()
{
	WeaponType = EWeaponType::Launcher;
	AttachSocketNameFPP = "S_Launcher";

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponObject(TEXT("SkeletalMesh'/Game/Weapons/Launcher/Launcher.Launcher'"));
	if (WeaponObject.Succeeded())
		WeaponMesh->SetSkeletalMesh(WeaponObject.Object);
}
