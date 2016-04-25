// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "GamePlayCharacter.generated.h"

UCLASS()
class SHOOTERGAME_API AGamePlayCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGamePlayCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	//////////////////////////////////////////////////////////////////
	// Input handlers

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void OnStartFire();

	void OnStopFire();

	void OnReload();
	//////////////////////////////////////////////////////////////////////////
	// Animations

	/** play anim montage */
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	/** stop playing montage */
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

	/** stop playing all montages */
	void StopAllAnimMontages();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FPPMesh;
public:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponSlot1;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponSlot2;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponSlot3;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	class AWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	bool isReloading;

	/**
	*	Is the equip animation is playing
	*/
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	bool bisChangingWeapon;

	/**	
	*	Used by the animation instance to play equipe animation
	*/
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	float WeaponPullDownPercent;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SpawnWeaponsAndAssignToSlots();

	UCameraComponent* GetCamera();

protected:

	virtual void PlayHit(bool bKilled, float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser) override;

	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;
};
