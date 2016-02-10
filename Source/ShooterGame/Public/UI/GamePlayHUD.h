// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "GamePlayHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGamePlayHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Main HUD update loop */
	void DrawHUD() override;
	
	/** Sent from ShooterWeapon, shows NO AMMO text. */
	void NotifyOutOfAmmo();

	AGamePlayHUD();
protected:
	/** Floor for automatic hud scaling. */
	static const float MinHudScale;

	/** Lighter HUD color */
	FColor HUDLight;

	/** Darker HUD color */
	FColor HUDDark;

	/** when we got the last notice about out of ammo */
	float NoAmmoNotifyTime;

	/** how long notice will fade out */
	float NoAmmoFadeOutTime;

	/** Primary weapon background icon. */
	UPROPERTY()
	FCanvasIcon PrimaryWeaponBg;

	/** Secondary weapon background icon */
	UPROPERTY()
	FCanvasIcon SecondaryWeaponBg;

	/** Crosshair Icons (Left, top, right, bottom, Center */
	UPROPERTY()
	FCanvasIcon CrossHair[5];

	/** Crosshair indicator that we hit somone */
	FCanvasIcon HitNotifyCrosshair;

	/** UI Scaling for other resolutions */
	float ScaleUI;

	/** FontRenderInfo enabling casting shadow */
	FFontRenderInfo ShadowedFont;

	UPROPERTY()
	UTexture2D* HUDMainTexture;

	UPROPERTY()
	UTexture2D* HUDAssets02Texture;

	UPROPERTY()
	UFont* BigFont;

	UPROPERTY()
	UFont* NormalFont;

	/** General offset for HUD elements */
	UPROPERTY()
	float offset;

	virtual void PostInitializeComponents() override;

	void DrawWeaponHUD();

	void DrawCrosshair();

	/** helper for getting uv coords in normalized top,left, bottom, right format */
	void MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);
};
