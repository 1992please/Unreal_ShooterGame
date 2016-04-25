// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class SHOOTERGAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/* Find a random point within the BoxComponent */
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

	/** this function toggles whether or not the spawn volume spawns pickups */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StopSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning(TArray< TSubclassOf<class ABaseCharacter> > &NewZombies, float NewMinDelay = 1.0f, float NewMaxDelay = 4.5f, int32 NoOfZombies = 1);

	bool IsSpawning();
protected:
	/* the pickup to spawn */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray< TSubclassOf<class ABaseCharacter> > ZombiesToSpawn;

	/* Minimum Spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MinSpawnDelay;

	/* Maximum spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MaxSpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 NoOfZombiesToSpawn;

	FTimerHandle SpawnTimer;
	
	/* Box Component to specify where pickups should be spawned */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* WhereToSpawn;

	/* Handle spawning a new pickup */
	void SpawnZombie();

	/* the current spawn delay */
	float SpawnDelay;

	void SpawnPickup();


	bool bIsSpawning;
};
