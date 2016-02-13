// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayHUD.h"
#include "Weapon.h"

const float AGamePlayHUD::MinHudScale = 0.5f;

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

	PrimaryWeaponBg = UCanvas::MakeIcon(HUDMainTexture, 543, 17, 441, 81);
	SecondaryWeaponBg = UCanvas::MakeIcon(HUDMainTexture, 676, 111, 293, 50);

	Crosshair[EShooterCrosshairDirection::Left] = UCanvas::MakeIcon(HUDMainTexture, 43, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Right] = UCanvas::MakeIcon(HUDMainTexture, 88, 402, 25, 9); // right
	Crosshair[EShooterCrosshairDirection::Top] = UCanvas::MakeIcon(HUDMainTexture, 74, 371, 9, 25); // top
	Crosshair[EShooterCrosshairDirection::Bottom] = UCanvas::MakeIcon(HUDMainTexture, 74, 415, 9, 25); // bottom
	Crosshair[EShooterCrosshairDirection::Center] = UCanvas::MakeIcon(HUDMainTexture, 75, 403, 7, 7); // center

	Offset = 20.0f;
	HUDLight = FColor(175, 202, 213, 255);
	HUDDark = FColor(110, 124, 131, 255);
	ShadowedFont.bEnableShadow = true;

}

void AGamePlayHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGamePlayHUD::NotifyOutOfAmmo()
{
	NoAmmoNotifyTime = GetWorld()->GetTimeSeconds();
}

void AGamePlayHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGamePlayHUD::DrawWeaponHUD()
{

}

void AGamePlayHUD::DrawCrosshair()
{
	AGamePlayPlayerController* PCOwner = Cast<AGamePlayPlayerController>(PlayerOwner);

	if (PCOwner)
	{
		AGamePlayCharacter* Pawn = Cast<AGamePlayCharacter>(PCOwner->GetPawn());
		if (Pawn)
		{
			const float SpreadMulti = 300;
			AWeapon* MyWeapon = Pawn->CurrentWeapon;
			const float CurrentTime = GetWorld()->GetTimeSeconds();
			if (MyWeapon)
			{
				float CrossSpread = 2 + SpreadMulti * FMath::Tan(MyWeapon->SpreadCurrent);
				float CenterX = Canvas->ClipX/2;
				float CenterY = Canvas->ClipY/2;

				Canvas->SetDrawColor(255, 255, 255, 192);

				FCanvasIcon* CurrentCrosshair[5];

				for (int32 i = 0; i < 5; i++)
				{
					// in Case you don't use custome cross hair for the weapon
					CurrentCrosshair[i] = &Crosshair[i];
				}
				// Start the drawing 
				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Center],
					CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).UL * ScaleUI/ 2.0f,
					CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).VL * ScaleUI/ 2.0f, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Bottom],
					CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Bottom]).UL * ScaleUI/ 2.0f,
					CenterY + CrossSpread * ScaleUI, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Left],
					CenterX- (*CurrentCrosshair[EShooterCrosshairDirection::Left]).UL * ScaleUI - CrossSpread * ScaleUI,
					CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Left]).VL * ScaleUI / 2.0f, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Right],
					CenterX + CrossSpread * ScaleUI,
					CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Right]).VL * ScaleUI / 2.0f, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Top],
					CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Top]).UL * ScaleUI / 2.0f,
					CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Top]).VL * ScaleUI - CrossSpread * ScaleUI, ScaleUI);


			}

		}
	}
}

void AGamePlayHUD::MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL)
{

}

void AGamePlayHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}
	// Get the new Ratio of the window
	ScaleUI = Canvas->ClipY / 1080.0f;

	ScaleUI = FMath::Max(ScaleUI, MinHudScale);

	
	DrawCrosshair();
}