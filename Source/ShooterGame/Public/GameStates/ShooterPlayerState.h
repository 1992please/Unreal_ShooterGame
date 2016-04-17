// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	int32 NumKills;

	UPROPERTY(Transient)
	int32 NumDeaths;

	UPROPERTY(Transient)
	int32 TeamNumber;

	virtual void Reset() override;

public:

	AShooterPlayerState();

	void AddKill();

	void AddDeath();
	
	void ScorePoints(int32 Points);

	void SetTeamNumber(int32 NewTeamNumber);

	UFUNCTION(BlueprintCallable, Category = "Teams")
	int32 GetTeamNumber() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetKills() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetDeaths() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	float GetScore() const;
};
