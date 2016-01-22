// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ShooterGameInstance.h"
#include "WeaponRifle.h"
#include "WeaponLauncher.h"
#include "WeaponShotGun.h"
UShooterGameInstance::UShooterGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageAsset1(TEXT("/Game/UI/Textures/0"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageAsset2(TEXT("/Game/UI/Textures/1"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageAsset3(TEXT("/Game/UI/Textures/2"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageAsset4(TEXT("/Game/UI/Textures/3"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageAsset5(TEXT("/Game/UI/Textures/4"));
	//TempItem.BackpackImage
	Backpack_Weapons.Add(FWeaponBackpackItem(ImageAsset1.Object, AWeaponRifle::StaticClass(), 1));
	Backpack_Weapons.Add(FWeaponBackpackItem(ImageAsset2.Object, AWeaponShotGun::StaticClass(), 2));
	Backpack_Weapons.Add(FWeaponBackpackItem(ImageAsset3.Object, AWeaponLauncher::StaticClass(), 3));
	Backpack_Weapons.Add(FWeaponBackpackItem(ImageAsset4.Object, AWeaponLauncher::StaticClass(), 0));
	Backpack_Weapons.Add(FWeaponBackpackItem(ImageAsset5.Object, AWeaponLauncher::StaticClass(),0));
}

void UShooterGameInstance::CanAddWEaponToWeaponSelected(bool& CanAdd, uint8& NoOfItemSelected)
{
	NoOfItemSelected = 0;
	CanAdd = false;
	for (const auto& Item : Backpack_Weapons)
	{
		if (Item.IsSelected)
		{
			NoOfItemSelected++;
		}
	}
	if (NoOfItemSelected < 3)
	{
		CanAdd = true;
	}
}

void UShooterGameInstance::SetBackpackItemSelected(uint8 BackPackItemIndex, bool IsSelected, uint8 WhichSlot)
{
	Backpack_Weapons[BackPackItemIndex].IsSelected = IsSelected;
	Backpack_Weapons[BackPackItemIndex].InSlot = WhichSlot;
}
