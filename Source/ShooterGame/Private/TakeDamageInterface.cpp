// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "TakeDamageInterface.h"


UTakeDamageInterface::UTakeDamageInterface(const FObjectInitializer& ObjectInit) : Super(ObjectInit)
{
}

void ITakeDamageInterface::TakeDamage(FAmmoData& AmmoData, FDamageModifier& DamageModifier, AActor* DamageCauser, FHitResult& FHitInfo)
{

}
