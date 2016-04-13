#include "STypes.generated.h"
#pragma once


#define COLLISION_WEAPON				ECC_GameTraceChannel1

#define SURFACE_DEFAULT				SurfaceType_Default
#define SURFACE_BODY				SurfaceType1
#define SURFACE_HEAD				SurfaceType2
#define SURFACE_LIMB				SurfaceType3

namespace EShooterCrosshairDirection
{
	enum Type
	{
		Left = 0,
		Right = 1,
		Top = 2,
		Bottom = 3,
		Center = 4
	};
}

namespace EShooterHudPosition
{
	enum Type
	{
		Left = 0,
		FrontLeft = 1,
		Front = 2,
		FrontRight = 3,
		Right = 4,
		BackRight = 5,
		Back = 6,
		BackLeft = 7,
	};
}

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

UENUM(BlueprintType)
enum class EAmmoType :uint8
{
	Conventional,
	Plasma,
	Laser
};

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

		/** inifite ammo for reloads */
		UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		bool bInfiniteAmmo;

	/** infinite ammo in clip, no reload required */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		bool bInfiniteClip;

	/** max ammo */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		int32 MaxAmmo;

	/** clip size */
	UPROPERTY(EditDefaultsOnly, Category = AmmoState)
		int32 AmmoPerClip;

	/** initial clips */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		int32 InitialClips;

	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float TimeBetweenShots;

	/** failsafe reload duration if weapon doesn't have any animation for it */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float NoAnimReloadDuration;
	/** defaults */
	FWeaponData()
	{
		bInfiniteAmmo = false;
		bInfiniteClip = false;
		MaxAmmo = 100;
		AmmoPerClip = 20;
		InitialClips = 4;
		TimeBetweenShots = 0.2f;
		NoAnimReloadDuration = 1.0f;
	}
};

USTRUCT()
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

		/** the type of ammo */
		UPROPERTY(EditDefaultsOnly, Category = AmmoState)
		EAmmoType AmmoType;

	/** The damage done by the ammo */
	UPROPERTY(EditDefaultsOnly, Category = AmmoState)
		float Damage;

	/** Crit Chance from 0 to 1 */
	UPROPERTY(EditDefaultsOnly, Category = AmmoState)
		float CritChance;

	/** the damage done over time if there's a damage */
	UPROPERTY(EditDefaultsOnly, Category = AmmoState)
		float DamageOverTime;

	/** how much time left for damage over time effect */
	UPROPERTY(EditDefaultsOnly, Category = AmmoState)
		float DamageOverTime_TimeLeft;

	/** Intialize some variables */
	FAmmoData()
	{
		AmmoType = EAmmoType::Conventional;
		Damage = 0;
		CritChance = 0;
		DamageOverTime = 0;
		DamageOverTime_TimeLeft = 0;
	}
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