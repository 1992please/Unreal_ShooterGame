// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DamageUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UDamageUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = MyVars)
	int32 Damage;

	UPROPERTY(BlueprintReadOnly, Category = MyVars)
	bool bWasCrit;
	
};
