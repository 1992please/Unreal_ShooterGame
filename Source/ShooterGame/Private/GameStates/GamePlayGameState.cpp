// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayGameState.h"


AGamePlayGameState::AGamePlayGameState()
{
	TotalScore = 0;
}


int32 AGamePlayGameState::GetTotalScore()
{
	return TotalScore;
}


void AGamePlayGameState::AddScore(int32 Score)
{
	TotalScore += Score;
}


