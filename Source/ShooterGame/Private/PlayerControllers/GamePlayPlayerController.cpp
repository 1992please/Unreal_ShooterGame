// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Blueprint/UserWidget.h"

AGamePlayPlayerController::AGamePlayPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentControllingDevice = EControllingDevice::Mouse;
	MouseSensitivityMin = 0.1f;
	MouseSensitivityMax = 2.f;
	MouseSensitivityCurrent = 1.f;
	TouchSensitivityMin = 15.f;
	TouchSensitivityMax = 5.f;
	TouchSensitivityCurrent = 10.f;
	GyroSensitivityMin = 20.f;
	GyroSensitivityMax = 60.f;
	GyroSensitivityCurrent = 40.f;

	// Set the widget
	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassAsset(TEXT("WidgetBlueprint'/Game/UI/UI_Debug_ChangeSensitivity'"));
	//if(WidgetClassAsset.Succeeded())
	//	ChangeSensitivityWidget = WidgetClassAsset.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassAsset1(TEXT("WidgetBlueprint'/Game/UI/Blueprints/UI_Debug_WeaponSelection'"));
	if (WidgetClassAsset1.Succeeded())
		WeaponSelectionWidget = WidgetClassAsset1.Class;
}

void AGamePlayPlayerController::Tick(float DeltaSeconds)
{
	//if (WasInputKeyJustPressed(EKeys::Q))
	//{
	//	UUserWidget*const TempWidget = CreateWidget<UUserWidget>(GetWorld(), ChangeSensitivityWidget);
	//	TempWidget->AddToViewport();
	//	FInputModeUIOnly Mode;
	//	SetInputMode(Mode);
	//	bShowMouseCursor = true;
	//}
	//if (WasInputKeyJustPressed(EKeys::R))
	//{
	//	UUserWidget*const TempWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponSelectionWidget);
	//	TempWidget->AddToViewport();
	//	FInputModeUIOnly Mode;
	//	SetInputMode(Mode);
	//	bShowMouseCursor = true;
	//}

	if (WasInputKeyJustPressed(EKeys::One))
	{
		AGamePlayCharacter* Character = Cast<AGamePlayCharacter>(GetPawn());
		if (Character)
		{
			Character->EquipWeapon(Character->WeaponSlot1);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Two))
	{
		AGamePlayCharacter* Character = Cast<AGamePlayCharacter>(GetPawn());
		if (Character)
		{
			Character->EquipWeapon(Character->WeaponSlot2);
		}
	}

	if (WasInputKeyJustPressed(EKeys::Three))
	{
		AGamePlayCharacter* Character = Cast<AGamePlayCharacter>(GetPawn());
		if (Character)
		{
			Character->EquipWeapon(Character->WeaponSlot3);
		}
	}
}

EControllingDevice AGamePlayPlayerController::GetCurrentControllingDevice() const
{
	return CurrentControllingDevice;
}

void AGamePlayPlayerController::SeCurrentState(const EControllingDevice NewControllingDevice)
{
	CurrentControllingDevice = NewControllingDevice;
}

float AGamePlayPlayerController::GetSensitivity() const
{
	switch (CurrentControllingDevice)
	{
		case EControllingDevice::Mouse:
			return MouseSensitivityCurrent;
			break;
		case EControllingDevice::Touch:
			return TouchSensitivityCurrent;
			break;
		case EControllingDevice::Gyro:
			return GyroSensitivityCurrent;
			break;
	}
	return MouseSensitivityCurrent;
}

void AGamePlayPlayerController::SetSensitivity(const float NewSensitivityCurrent)
{
	switch (CurrentControllingDevice)
	{
		case EControllingDevice::Mouse:
			MouseSensitivityCurrent = NewSensitivityCurrent;
			break;
		case EControllingDevice::Touch:
			TouchSensitivityCurrent = NewSensitivityCurrent;
			break;
		case EControllingDevice::Gyro:
			GyroSensitivityCurrent = NewSensitivityCurrent;
			break;
	}
}

void AGamePlayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	switch (CurrentControllingDevice)
	{
		case EControllingDevice::Mouse:
			InputComponent->BindAxis("MouseX", this, &AGamePlayPlayerController::InputAxisMouseX);
			InputComponent->BindAxis("MouseY", this, &AGamePlayPlayerController::InputAxisMouseY);
			break;
		case EControllingDevice::Touch:
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, TEXT("We don't support touch screens yet"));
			break;
		case EControllingDevice::Gyro:
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, TEXT("We don't support touch screens yet"));
			break;
	}
}

void AGamePlayPlayerController::InputAxisMouseY(float AxisVal)
{
	AddPitchInput(AxisVal * MouseSensitivityCurrent);
}

void AGamePlayPlayerController::InputAxisMouseX(float AxisVal)
{
	AddYawInput(AxisVal * MouseSensitivityCurrent);
}