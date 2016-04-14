// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayHUD.h"

AGamePlayGameMode::AGamePlayGameMode()
{
	/*static ConstructorHelpers::FClassFinder<APawn> PawnAsset(TEXT("/Game/Blueprints/BP_GamePlayCharacter"));
	if (PawnAsset.Succeeded())
		DefaultPawnClass = PawnAsset.Class;*/
	PlayerControllerClass = AGamePlayPlayerController::StaticClass();
	GameStateClass = AGamePlayGameState::StaticClass();
	HUDClass = AGamePlayHUD::StaticClass();
}

void AGamePlayGameMode::Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType)
{
	// Do nothing (can we used to apply score or keep track of kill count)
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Enemy Killed");
}
