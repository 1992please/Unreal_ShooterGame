// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "WeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AWeaponRifle : public AWeapon
{
	GENERATED_BODY()
	
public:
	AWeaponRifle();
	
	void Fire() override;
	void Reload() override;
};
