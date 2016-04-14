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
	UPROPERTY()
	class AWeapon* WeaponSlot1;
	UPROPERTY()
	class AWeapon* WeaponSlot2;
	UPROPERTY()
	class AWeapon* WeaponSlot3;

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
	void EquipWeapon(class AWeapon* WhichWeapon);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SpawnWeaponsAndAssignToSlots();

	UCameraComponent* GetCamera();

protected:

	UFUNCTION(BlueprintPure, Category = Utility)
	class UShooterGameInstance* GetShooterGameInstance();

	/**
	*	Show the weapon as an Event in the middle of the animation
	*/
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void ShowWeapon(class AWeapon* WeaponToShow);

	FTimeline Timeline;
	UFUNCTION()
	void TimeLineChangeProgress(float Value);
	UFUNCTION()
	void TimeLineWeaponDown();

private:
	UPROPERTY()
	class AWeapon* BindingToBeEquiped;
};
