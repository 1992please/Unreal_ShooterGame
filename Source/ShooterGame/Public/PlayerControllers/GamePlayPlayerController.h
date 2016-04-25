// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "STypes.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float MouseSensitivityMin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float MouseSensitivityMax;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controls)
	float MouseSensitivityCurrent;

public:

	UFUNCTION(BlueprintPure, Category = Controls)
	float GetSensitivity() const;

	UFUNCTION(BlueprintCallable, Category = Controls)
	void SetSensitivity(const float NewSensitivityCurrent);

protected:
	virtual void SetupInputComponent() override;

private:
	void InputAxisMouseY(float AxisVal);
	void InputAxisMouseX(float AxisVal);

};
