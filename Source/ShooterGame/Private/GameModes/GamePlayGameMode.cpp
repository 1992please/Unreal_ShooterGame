// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayHUD.h"
#include "ShooterPlayerState.h"
#include "ZombieAIController.h"
#include "EnemyZombie.h"
#include "SpawnVolume.h"


AGamePlayGameMode::AGamePlayGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
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

	UWorld* World = GetWorld();
	SpawnVolumes.Empty();
	// Initialize the SpawnVolumes Array
	if (World)
	{
		for (TActorIterator<ASpawnVolume>It(World); It; ++It)
		{
			SpawnVolumes.Add(*It);
		}
	}

	// for test
	SpawnVolumes[0]->StartSpawning(BotsToSpawn, .5f, .5f, 2);

	CurrentState = EPlayState::EPlaying;
}

void AGamePlayGameMode::Tick(float DeltaSeconds)
{
}

void AGamePlayGameMode::SetGameState(EPlayState NewPlayState)
{
	CurrentState = NewPlayState;

	HandleCurrentState();
}

void AGamePlayGameMode::HandleCurrentState()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AGamePlayHUD* HUD = Cast<AGamePlayHUD>(World->GetFirstPlayerController()->GetHUD());
		if (HUD)
		{
			HUD->SetPlayState(CurrentState);
		}
	}

	
}

void AGamePlayGameMode::HandleCurrentGamePlayStage()
{
	switch (CurrentGameStage)
	{
		case EGamePlayState::Stage1:
		{

		}
		break;
		case EGamePlayState::Stage2:
		{

		}
		break;
		case EGamePlayState::Stage3:
		{

		}
		break;
		case EGamePlayState::Stage4:
		{

		}
		break;
		default:
		{

		}
		break;
	}
}

void AGamePlayGameMode::Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType)
{
	AShooterPlayerState* VictimPS = Cast<AShooterPlayerState>(VictimPawn->PlayerState);
	AShooterPlayerState* KillerPS = Cast<AShooterPlayerState>(Killer->PlayerState);
	// Do nothing (can we used to apply score or keep track of kill count)
	if (VictimPS && VictimPS->bIsABot)
	{
		KillerPS->AddKill();
		if (CheckFinishedSpawning() && !CheckAnyZombieAlive())
		{
			SetGameState(EPlayState::EWon);
		}
	}
	else
	{
		SetGameState(EPlayState::EGameOver);
	}
}

bool AGamePlayGameMode::CheckFinishedSpawning()
{
	for (auto& SpawnVolume : SpawnVolumes)
	{
		if (SpawnVolume->IsSpawning())
		{
			return false;
		}
	}

	return true;
}

bool AGamePlayGameMode::CheckAnyZombieAlive()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<AEnemyZombie>It(World); It; ++It)
		{
			if (!It->IsDying())
			{
				return true;
			}
		}
	}

	return false;
}