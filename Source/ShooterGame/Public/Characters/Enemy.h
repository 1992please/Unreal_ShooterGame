// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.generated.h"

UCLASS()
class SHOOTERGAME_API AEnemy : public ACharacter
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

	void CalculateDamage(float Damage, float CritChance, float CritDamageModifier, FHitResult& HitInfo, float& OutDamageTaken, bool& OutWasCrit);

	/* Take damage & handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
};
