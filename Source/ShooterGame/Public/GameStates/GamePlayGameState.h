// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GamePlayGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGamePlayGameState : public AGameState
{
	GENERATED_BODY()
	
	int32 TotalScore;
	
public:

	AGamePlayGameState();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetTotalScore();

	void AddScore(int32 Score);
};
