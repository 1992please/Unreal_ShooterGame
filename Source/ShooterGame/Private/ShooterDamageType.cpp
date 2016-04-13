// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ShooterDamageType.h"


UShooterDamageType::UShooterDamageType()
{
	/* We apply this modifier based on the physics material setup to the head of the enemy PhysAsset */
	HeadDmgModifier = 2.0f;
	LimbDmgModifier = 0.5f;

	bCanDieFrom = true;
}



bool UShooterDamageType::GetCanDieFrom()
{
	return bCanDieFrom;
}


float UShooterDamageType::GetHeadDamageModifier()
{
	return HeadDmgModifier;
}

float UShooterDamageType::GetLimbDamageModifier()
{
	return LimbDmgModifier;
}