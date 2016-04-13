// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "ShooterDamageType.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UShooterDamageType : public UDamageType
{
	GENERATED_BODY()
	
	UShooterDamageType();

	/* Can player die from this damage type (eg. players don't die from hunger) */
	UPROPERTY(EditDefaultsOnly)
	bool bCanDieFrom;

	/* Damage modifier for headshot damage */
	UPROPERTY(EditDefaultsOnly)
	float HeadDmgModifier;

	UPROPERTY(EditDefaultsOnly)
	float LimbDmgModifier;

public:

	bool GetCanDieFrom();

	float GetHeadDamageModifier();

	float GetLimbDamageModifier();
	
	
};
