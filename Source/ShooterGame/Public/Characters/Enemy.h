// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TakeDamageInterface.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTERGAME_API AEnemy : public ACharacter , public ITakeDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	bool isDead;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = States)
	float MaxHealth;

	// returns true if the player is dead or not
	bool HealthToModify(float HealthToModify);

	void CalculateDamage(float Damage, float CritChance, float CritDamageModifier, FHitResult& HitInfo, float& OutDamageTaken, bool& OutWasCrit);
};
