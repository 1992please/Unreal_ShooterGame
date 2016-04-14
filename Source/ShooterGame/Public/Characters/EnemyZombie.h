// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "EnemyZombie.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AEnemyZombie : public AEnemy
{
	GENERATED_BODY()
	
	/* Last time we attacked something */
	float LastMeleeAttackTime;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual bool IsSprinting() const;
	
	UPROPERTY(VisibleAnywhere, Category = "Attacking")
	UCapsuleComponent* MeleeCollisionComp;

	/* A pawn is in melee range */
	UFUNCTION()
	void OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void OnRetriggerMeleeStrike();

	/* Deal damage to the Actor that was hit by the punch animation */
	UFUNCTION(BlueprintCallable, Category = "Attacking")
	void PerformMeleeStrike(AActor* HitActor);

	void SimulateMeleeStrike();

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	TSubclassOf<UDamageType> PunchDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	UAnimMontage* MeleeAnimMontage;

	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MeleeAttack;

	/* Minimum time between melee attacks */
	float MeleeStrikeCooldown;

	/* Plays the idle, wandering or hunting sound */
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioLoopComp;


	UAudioComponent* PlayCharacterSound(USoundCue* CueToPlay);

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundPlayerNoticed;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundHunting;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundIdle;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundWandering;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundAttackMelee;

public:

	AEnemyZombie();

	UPROPERTY(BlueprintReadWrite, Category = "Attacking")
	bool bIsPunching;
};
