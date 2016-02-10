// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayHUD.h"

AGamePlayHUD::AGamePlayHUD()
{
	NoAmmoFadeOutTime = 1.0f;
	NoAmmoNotifyTime = -NoAmmoFadeOutTime;

	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/Game/UI/HUD/HUDMain"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDAssets02TextureOb(TEXT("/Game/UI/HUD/HUDAssets02"));

	static ConstructorHelpers::FObjectFinder<UFont> BigFontOb(TEXT("/Game/UI/HUD/Roboto51"));
	static ConstructorHelpers::FObjectFinder<UFont> NormalFontOb(TEXT("/Game/UI/HUD/Roboto18"));

	HUDMainTexture = HUDMainTextureOb.Object;
	HUDAssets02Texture = HUDAssets02TextureOb.Object;
	BigFont = BigFontOb.Object;
	NormalFont = NormalFontOb.Object;
}

void AGamePlayHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

void AGamePlayHUD::NotifyOutOfAmmo()
{

}

void AGamePlayHUD::PostInitializeComponents()
{

}

void AGamePlayHUD::DrawWeaponHUD()
{

}

void AGamePlayHUD::DrawCrosshair()
{

}

void AGamePlayHUD::MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL)
{

}

void AGamePlayHUD::DrawHUD()
{
	Super::DrawHUD();

	//DrawRect(FLinearColor::Red, Canvas->SizeX/2 - 1, Canvas->SizeY/2 - 1, 2,2);
	
}