#include "STypes.generated.h"
#pragma once

UENUM(BlueprintType)
enum class EControllingDevice : uint8
{
	Mouse,
	Touch,
	Gyro
};

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	Rifle,
	Shotgun,
	Launcher
};

USTRUCT(BlueprintType)
struct FWeaponBackpackItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeapon> WeaponToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BackpackImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 InSlot;

	FWeaponBackpackItem()
	{
		WeaponToSpawn = NULL;
		BackpackImage = NULL;
		IsSelected = false;
		InSlot = 0;
	}

	FWeaponBackpackItem(UTexture2D* Image, TSubclassOf<class AWeapon> Weapon, int inSlot)
	{
		WeaponToSpawn = Weapon;
		BackpackImage = Image;
		IsSelected = false;
		InSlot = inSlot;
	}

	void Destroy()
	{
		WeaponToSpawn = nullptr;
		BackpackImage = nullptr;
	}
};