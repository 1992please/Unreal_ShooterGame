// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "STypes.h"
#include "GamePlayHUD.generated.h"

struct FHitData
{
	/** Last hit time. */
	float HitTime;

	/** strength of hit icon */
	float HitPercentage;

	/** Initialise defaults. */
	FHitData()
	{
		HitTime = 0.0f;
		HitPercentage = 0.0f;
	}
};

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

	void SetPlayState(EPlayState NewPlayState);

	/**
	* Sent from pawn hit, used to calculate hit notification overlay for drawing.
	*
	* @param	DamageTaken		The amount of damage.
	* @param	DamageEvent		The actual damage event.
	* @param	PawnInstigator	The pawn that did the damage.
	*/
	void NotifyGotHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator);

protected:
	EPlayState PlayState;

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

	/** Most recent hit time, used to check if we need to draw hit indicator at all. */
	float LastHitTime;

	/** How long till hit notify fades out completely. */
	float HitNotifyDisplayTime;

	/** Primary weapon background icon. */
	UPROPERTY()
	FCanvasIcon PrimaryWeaponBg;

	/** Secondary weapon background icon */
	UPROPERTY()
	FCanvasIcon SecondaryWeaponBg;

	/** Icons for hit indicator. */
	UPROPERTY()
	FCanvasIcon HitNotifyIcon[8];

	/** kills background icon. */
	UPROPERTY()
	FCanvasIcon KillsBg;

	/** Crosshair Icons (Left, top, right, bottom, Center */
	UPROPERTY()
	FCanvasIcon Crosshair[5];

	/** Health bar background icon. */
	UPROPERTY()
	FCanvasIcon HealthBarBg;

	/** Health bar icon. */
	UPROPERTY()
	FCanvasIcon HealthBar;

	/** Kills icon. */
	UPROPERTY()
	FCanvasIcon KillsIcon;

	/** Health icon on the health bar. */
	UPROPERTY()
	FCanvasIcon HealthIcon;

	/** Crosshair indicator that we hit somone */
	FCanvasIcon HitNotifyCrosshair;

	/** Death messages background icon. */
	UPROPERTY()
	FCanvasIcon DeathMessagesBg;

	/** UI Scaling for other resolutions */
	float ScaleUI;

	/** FontRenderInfo enabling casting shadow */
	FFontRenderInfo ShadowedFont;

	UPROPERTY()
	UTexture2D* HUDMainTexture;

	UPROPERTY()
	UTexture2D* HUDAssets02Texture;

	/** Texture for hit indicator. */
	UPROPERTY()
	UTexture2D* HitNotifyTexture;

	UPROPERTY()
	UFont* BigFont;

	UPROPERTY()
	UFont* NormalFont;

	/** General Offset for HUD elements */
	UPROPERTY()
	float Offset;

	/** Offsets to display hit indicator parts. */
	FVector2D Offsets[8];

	/** Runtime data for hit indicator. */
	FHitData HitNotifyData[8];

	virtual void PostInitializeComponents() override;



	/** Draws hit indicator. */
	void DrawHitIndicator();

	void DrawHealth();

	void DrawKills();

	void DrawWeaponHUD();

	void DrawCrosshair();

	void DrawEndOfGameScreen();

	/** helper for getting uv coords in normalized top,left, bottom, right format */
	void MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);
};
