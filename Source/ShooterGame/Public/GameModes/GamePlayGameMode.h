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
	
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TArray< TSubclassOf<class ABaseCharacter>> BotsToSpawn;

	virtual void Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType);

	/** returns default pawn class for given controller */
	//virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void SpawnNewBot(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<class ABaseCharacter> Bot);
};
