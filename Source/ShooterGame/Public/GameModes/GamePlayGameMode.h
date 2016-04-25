// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "STypes.h"
#include "GameFramework/GameMode.h"
#include "GamePlayGameMode.generated.h"


enum class EGamePlayState :uint8
{
	Stage1,
	Stage2,
	Stage3,
	Stage4
};

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGamePlayGameMode : public AGameMode
{
	GENERATED_BODY()
	AGamePlayGameMode();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TArray< TSubclassOf<class ABaseCharacter>> BotsToSpawn;

	virtual void Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType);

	/** returns default pawn class for given controller */
	//virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;


	bool CheckAnyZombieAlive();

	bool CheckFinishedSpawning();

	void SetGameState(EPlayState NewPlayState);
	
	void HandleCurrentState();

	void HandleCurrentGamePlayStage();
private:
	TArray<class ASpawnVolume*> SpawnVolumes;  

	EPlayState CurrentState;
	EGamePlayState CurrentGameStage;
};
