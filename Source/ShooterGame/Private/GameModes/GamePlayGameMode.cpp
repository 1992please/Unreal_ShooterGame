// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayGameMode.h"
#include "GamePlayCharacter.h"
#include "GamePlayPlayerController.h"
#include "GamePlayGameState.h"
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

