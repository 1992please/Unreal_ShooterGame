// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ShooterPlayerState.h"

AShooterPlayerState::AShooterPlayerState()
{
	/* AI will remain in team 0, players are updated to team 1 through the GameMode::InitNewPlayer */
	TeamNumber = 0;
}


void AShooterPlayerState::Reset()
{
	Super::Reset();

	NumKills = 0;
	NumDeaths = 0;
	Score = 0;
}

void AShooterPlayerState::AddKill()
{
	NumKills++;
}

void AShooterPlayerState::AddDeath()
{
	NumDeaths++;
}

void AShooterPlayerState::ScorePoints(int32 Points)
{
	Score += Points;

	/* Add the score to the global score count */
	AGamePlayGameState* GS = Cast<AGamePlayGameState>(GetWorld()->GameState);
	if (GS)
	{
		GS->AddScore(Points);
	}
}

void AShooterPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}

int32 AShooterPlayerState::GetTeamNumber() const
{
	return TeamNumber;
}

int32 AShooterPlayerState::GetKills() const
{
	return NumKills;
}

int32 AShooterPlayerState::GetDeaths() const
{
	return NumDeaths;
}


float AShooterPlayerState::GetScore() const
{
	return Score;
}