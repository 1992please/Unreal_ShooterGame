// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TakeDamageInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class UTakeDamageInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ITakeDamageInterface
{
	GENERATED_IINTERFACE_BODY()

	void TakeDamage(FAmmoData& AmmoData, FDamageModifier& DamageModifier, AActor* DamageCauser, FHitResult& FHitInfo);
};