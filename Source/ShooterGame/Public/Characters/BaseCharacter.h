// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SHOOTERGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundTakeHit;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundDeath;

	UFUNCTION(BlueprintCallable, Category = "CharacterCondition")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterCondition")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterCondition")
	bool IsAlive() const;

	bool IsDying();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
	float Health;

	/* Take damage & handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual bool CanDie() const;

	virtual bool Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

	virtual void ApplyPhysicsToTheRagdolledBody(FDamageEvent const& DamageEvent);

	virtual void FellOutOfWorld(const class UDamageType& DmgType) override;

	virtual void SetRagdollPhysics();

	virtual void PlayHit(bool bKilled , float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	bool bIsDying;
};
