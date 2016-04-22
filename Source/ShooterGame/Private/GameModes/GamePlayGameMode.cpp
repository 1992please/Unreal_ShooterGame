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
	SpawnNewBot(Cast<ASpawnVolume>(SV[0])->GetRandomPointInVolume(), FRotator::ZeroRotator, BotToSpawn);
}

void AGamePlayGameMode::Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType)
{
	// Do nothing (can we used to apply score or keep track of kill count)
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Enemy Killed");
}

void AGamePlayGameMode::SpawnNewBot(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<AEnemyZombie> Bot)
{
	UWorld* const World = GetWorld();
	if (World && Bot)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyZombie* Zombie = World->SpawnActor<AEnemyZombie>(Bot, SpawnLocation, SpawnRotation, SpawnInfo);
		Zombie->SpawnDefaultController();
	}
}