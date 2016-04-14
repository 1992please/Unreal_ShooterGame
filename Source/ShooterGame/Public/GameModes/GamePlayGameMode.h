// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GamePlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGamePlayGameMode : public AGameMode
{
	GENERATED_BODY()
	AGamePlayGameMode();
	
	
public:
	virtual void Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType);
};
