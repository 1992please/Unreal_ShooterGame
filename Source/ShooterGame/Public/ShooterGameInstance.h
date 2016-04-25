// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "STypes.h"
#include "Engine/GameInstance.h"
#include "ShooterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UShooterGameInstance();
public:


	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	FAmmoData AmmoConventional;
};
