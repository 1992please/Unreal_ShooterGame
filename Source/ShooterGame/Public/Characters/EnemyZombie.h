// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "EnemyZombie.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AEnemyZombie : public ABaseCharacter
{
	GENERATED_BODY()
	
	/* Last time we attacked something */
	float LastMeleeAttackTime;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;


protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Attacking")
	UCapsuleComponent* MeleeCollisionComp;

	/* A pawn is in melee range */
	UFUNCTION()
	void OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void PlaySoundLoop();

	void StartAttackingPlayer();

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
	TArray<UAnimMontage*> MeleeAnimMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attacked")
	UAnimMontage* BodyHitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attacked")
	UAnimMontage* HeadHitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attacked")
	UAnimMontage* DefaultHitAnimMontage;




	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MeleeAttack;

	/* Minimum time between melee attacks */
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float MeleeStrikeCooldown;

	/* Plays the idle, wandering or hunting sound */
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioLoopComp;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	float StartUpTime;


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

	virtual void PlayHit(bool bKilled, float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser) override;
public:

	AEnemyZombie();

	UPROPERTY(BlueprintReadWrite, Category = "Attacking")
	bool bIsPunching;

	/* The thinking part of the brain, steers our zombie and makes decisions based on the data we feed it from the Blackboard */
	/* Assigned at the Character level (instead of Controller) so we may use different zombie behaviors while re-using one controller. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
};
