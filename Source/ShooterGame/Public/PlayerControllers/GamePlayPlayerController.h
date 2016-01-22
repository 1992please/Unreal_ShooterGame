// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "STypes.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGamePlayPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

private:

	AGamePlayPlayerController();

protected:
	/**
	*	Some Control Properties
	*/
	EControllingDevice CurrentControllingDevice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float MouseSensitivityMin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float MouseSensitivityMax;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float MouseSensitivityCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float TouchSensitivityMin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float TouchSensitivityMax;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float TouchSensitivityCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float GyroSensitivityMin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float GyroSensitivityMax;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float GyroSensitivityCurrent;

public:
	UFUNCTION(BlueprintPure, Category = Controls)
	EControllingDevice GetCurrentControllingDevice() const;

	UFUNCTION(BlueprintCallable, Category = Controls)
	void SeCurrentState(const EControllingDevice NewControllingDevice);

	UFUNCTION(BlueprintPure, Category = Controls)
	float GetSensitivity() const;

	UFUNCTION(BlueprintCallable, Category = Controls)
	void SetSensitivity(const float NewSensitivityCurrent);

protected:
	virtual void SetupInputComponent() override;

private:
	void InputAxisMouseY(float AxisVal);
	void InputAxisMouseX(float AxisVal);

public:
	UPROPERTY()
	TSubclassOf<class UUserWidget> ChangeSensitivityWidget;
	UPROPERTY()
	TSubclassOf<class UUserWidget> WeaponSelectionWidget;
};
