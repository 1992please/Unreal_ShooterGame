// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "SpawnVolume.h"
#include "BaseCharacter.h"
#include "Components/ArrowComponent.h"
// TODO Remove Auto Spawn Zombie Functionality
// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Spawn Box
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;
	WhereToSpawn->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create ArrowComponent
	UArrowComponent*const Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	Arrow->AttachParent = RootComponent;

	// Set the SpawnDelay
	MaxSpawnDelay = 4.5f;
	MinSpawnDelay = 1.0f;
	NoOfZombiesToSpawn = 1;
	bIsSpawning = false;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	//SpawnZombie();
}

bool ASpawnVolume::IsSpawning()
{
	return bIsSpawning;
}

void ASpawnVolume::StartSpawning(TArray< TSubclassOf<class ABaseCharacter> > &NewZombies, float NewMinDelay, float NewMaxDelay, int32 NoOfZombies)
{
	MaxSpawnDelay = NewMaxDelay;
	MinSpawnDelay = NewMinDelay;
	NoOfZombiesToSpawn = NoOfZombies;

	ZombiesToSpawn = NewZombies;
	// Set the timer on spawn active
	SpawnDelay = FMath::FRandRange(MinSpawnDelay, MaxSpawnDelay);
	SpawnTimer.Invalidate();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnZombie, SpawnDelay, false);
	bIsSpawning = true;
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FBox Box = WhereToSpawn->Bounds.GetBox();
	return FMath::RandPointInBox(Box);
}

void ASpawnVolume::StopSpawning()
{
	// clear timer on Spawn Pickup
	SpawnTimer.Invalidate();
}

void ASpawnVolume::SpawnZombie()
{
	if (ZombiesToSpawn.Num() > 0 && NoOfZombiesToSpawn > 0)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			//AGamePlayGameMode* const GameMode = Cast<AGamePlayGameMode>(World->GetAuthGameMode());
			for (TSubclassOf<ABaseCharacter> WhatToSpawn : ZombiesToSpawn)
			{
				// if we have set something to spawn;
				if (WhatToSpawn != NULL)
				{
					// check for a valid world

						// set the spawn parameters
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = Instigator;

					//Get a random location to spawn at
					FVector SpawnLocation = GetRandomPointInVolume();
					FRotator SpawnRotation = FRotator::ZeroRotator;
					// spawn the pickup
					ABaseCharacter* const Character = World->SpawnActor<ABaseCharacter>(WhatToSpawn, SpawnLocation, GetActorRotation(), SpawnParams);
					Character->SpawnDefaultController();
					//GameMode->SpawnNewBot(SpawnLocation, SpawnRotation, WhatToSpawn);
					if (--NoOfZombiesToSpawn > 0)
					{
						SpawnDelay = FMath::FRandRange(MinSpawnDelay, MaxSpawnDelay);
						GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnZombie, SpawnDelay, false);
					}
					else
					{
						bIsSpawning = false;
					}
				}
			}
		}
	}
}