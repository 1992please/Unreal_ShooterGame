// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "STypes.h"
#include "Engine/GameInstance.h"
#include "ShooterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UShooterGameInstance();
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FWeaponBackpackItem> Backpack_Weapons;
	
	/**
	*	This function will tell us if there is a space in player inventory to select another 
	*	@param NoOfItemSelected no of selected items
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void CanAddWEaponToWeaponSelected(bool& CanAdd, uint8& NoOfItemSelected);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void SetBackpackItemSelected(uint8 BackPackItemIndex, bool IsSelected, uint8 WhichSlot);


	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	FAmmoData AmmoConventional;
};
