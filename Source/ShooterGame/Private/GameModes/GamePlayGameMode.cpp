// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayHUD.h"
#include "ShooterPlayerState.h"
#include "ZombieAIController.h"
#include "EnemyZombie.h"
#include "SpawnVolume.h"


AGamePlayGameMode::AGamePlayGameMode()
{
	/*static ConstructorHelpers::FClassFinder<APawn> PawnAsset(TEXT("/Game/Blueprints/BP_GamePlayCharacter"));
	if (PawnAsset.Succeeded())
		DefaultPawnClass = PawnAsset.Class;*/
	PlayerStateClass = AShooterPlayerState::StaticClass();
	PlayerControllerClass = AGamePlayPlayerController::StaticClass();
	GameStateClass = AGamePlayGameState::StaticClass();
	HUDClass = AGamePlayHUD::StaticClass();
}

void AGamePlayGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> SV;
	UGameplayStatics::GetAllActorsOfClass(this, ASpawnVolume::StaticClass(), SV);
	Cast<ASpawnVolume>(SV[0])->StartSpawning(BotsToSpawn, 1, 1, 1);
}

void AGamePlayGameMode::Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType)
{
	AShooterPlayerState* VictimPS = Cast<AShooterPlayerState>(VictimPawn->PlayerState);
	AShooterPlayerState* KillerPS = Cast<AShooterPlayerState>(Killer->PlayerState);
	// Do nothing (can we used to apply score or keep track of kill count)
	if (VictimPS && VictimPS->bIsABot)
	{
		KillerPS->AddKill();
	}
}

void AGamePlayGameMode::SpawnNewBot(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ABaseCharacter> Bot)
{
	UWorld* const World = GetWorld();
	if (World && Bot)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ABaseCharacter* Zombie = World->SpawnActor<ABaseCharacter>(Bot, SpawnLocation, SpawnRotation, SpawnInfo);
		Zombie->SpawnDefaultController();
	}
}