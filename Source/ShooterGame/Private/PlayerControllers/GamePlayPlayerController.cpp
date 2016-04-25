// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Blueprint/UserWidget.h"

AGamePlayPlayerController::AGamePlayPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	MouseSensitivityMin = 0.1f;
	MouseSensitivityMax = 2.f;
	MouseSensitivityCurrent = 1.f;


	// Set the widget
	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassAsset(TEXT("WidgetBlueprint'/Game/UI/UI_Debug_ChangeSensitivity'"));
	//if(WidgetClassAsset.Succeeded())
	//	ChangeSensitivityWidget = WidgetClassAsset.Class;

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
}

float AGamePlayPlayerController::GetSensitivity() const
{
	return MouseSensitivityCurrent;
}

void AGamePlayPlayerController::SetSensitivity(const float NewSensitivityCurrent)
{
	MouseSensitivityCurrent = NewSensitivityCurrent;
}

void AGamePlayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MouseX", this, &AGamePlayPlayerController::InputAxisMouseX);
	InputComponent->BindAxis("MouseY", this, &AGamePlayPlayerController::InputAxisMouseY);
}

void AGamePlayPlayerController::InputAxisMouseY(float AxisVal)
{
	AddPitchInput(AxisVal * MouseSensitivityCurrent);
}

void AGamePlayPlayerController::InputAxisMouseX(float AxisVal)
{
	AddYawInput(AxisVal * MouseSensitivityCurrent);
}