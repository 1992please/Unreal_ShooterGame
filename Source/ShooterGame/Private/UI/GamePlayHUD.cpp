// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayHUD.h"
#include "Weapon.h"
#include "ShooterPlayerState.h"

const float AGamePlayHUD::MinHudScale = 0.5f;

AGamePlayHUD::AGamePlayHUD()
{
	NoAmmoFadeOutTime = 1.0f;
	HitNotifyDisplayTime = 0.75f;
	NoAmmoNotifyTime = -NoAmmoFadeOutTime;
	LastHitTime = -1;
	static ConstructorHelpers::FObjectFinder<UTexture2D> HitTextureOb(TEXT("/Game/UI/HUD/HitIndicator"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/Game/UI/HUD/HUDMain"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDAssets02TextureOb(TEXT("/Game/UI/HUD/HUDAssets02"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> LowHealthOverlayTextureOb(TEXT("/Game/UI/HUD/LowHealthOverlay"));

	static ConstructorHelpers::FObjectFinder<UFont> BigFontOb(TEXT("/Game/UI/HUD/Roboto51"));
	static ConstructorHelpers::FObjectFinder<UFont> NormalFontOb(TEXT("/Game/UI/HUD/Roboto18"));

	HitNotifyTexture = HitTextureOb.Object;
	HUDMainTexture = HUDMainTextureOb.Object;
	HUDAssets02Texture = HUDAssets02TextureOb.Object;
	BigFont = BigFontOb.Object;
	NormalFont = NormalFontOb.Object;

	PrimaryWeaponBg = UCanvas::MakeIcon(HUDMainTexture, 543, 17, 441, 81);
	SecondaryWeaponBg = UCanvas::MakeIcon(HUDMainTexture, 676, 111, 293, 50);

	HitNotifyIcon[EShooterHudPosition::Left] = UCanvas::MakeIcon(HitNotifyTexture, 158, 831, 585, 392);
	HitNotifyIcon[EShooterHudPosition::FrontLeft] = UCanvas::MakeIcon(HitNotifyTexture, 369, 434, 460, 378);
	HitNotifyIcon[EShooterHudPosition::Front] = UCanvas::MakeIcon(HitNotifyTexture, 848, 284, 361, 395);
	HitNotifyIcon[EShooterHudPosition::FrontRight] = UCanvas::MakeIcon(HitNotifyTexture, 1212, 397, 427, 394);
	HitNotifyIcon[EShooterHudPosition::Right] = UCanvas::MakeIcon(HitNotifyTexture, 1350, 844, 547, 321);
	HitNotifyIcon[EShooterHudPosition::BackRight] = UCanvas::MakeIcon(HitNotifyTexture, 1232, 1241, 458, 341);
	HitNotifyIcon[EShooterHudPosition::Back] = UCanvas::MakeIcon(HitNotifyTexture, 862, 1384, 353, 408);
	HitNotifyIcon[EShooterHudPosition::BackLeft] = UCanvas::MakeIcon(HitNotifyTexture, 454, 1251, 371, 410);

	Crosshair[EShooterCrosshairDirection::Left] = UCanvas::MakeIcon(HUDMainTexture, 43, 402, 25, 9);
	Crosshair[EShooterCrosshairDirection::Right] = UCanvas::MakeIcon(HUDMainTexture, 88, 402, 25, 9); // right
	Crosshair[EShooterCrosshairDirection::Top] = UCanvas::MakeIcon(HUDMainTexture, 74, 371, 9, 25); // top
	Crosshair[EShooterCrosshairDirection::Bottom] = UCanvas::MakeIcon(HUDMainTexture, 74, 415, 9, 25); // bottom
	Crosshair[EShooterCrosshairDirection::Center] = UCanvas::MakeIcon(HUDMainTexture, 75, 403, 7, 7); // center

	KillsBg = UCanvas::MakeIcon(HUDMainTexture, 15, 16, 235, 62);
	HealthIcon = UCanvas::MakeIcon(HUDAssets02Texture, 78, 262, 28, 28);
	KillsIcon = UCanvas::MakeIcon(HUDMainTexture, 318, 93, 24, 24);

	HealthBar = UCanvas::MakeIcon(HUDAssets02Texture, 67, 212, 372, 50);
	HealthBarBg = UCanvas::MakeIcon(HUDAssets02Texture, 67, 162, 372, 50);

	Offsets[EShooterHudPosition::Left] = FVector2D(173, 0);
	Offsets[EShooterHudPosition::FrontLeft] = FVector2D(120, 125);
	Offsets[EShooterHudPosition::Front] = FVector2D(0, 173);
	Offsets[EShooterHudPosition::FrontRight] = FVector2D(-120, 125);
	Offsets[EShooterHudPosition::Right] = FVector2D(-173, 0);
	Offsets[EShooterHudPosition::BackRight] = FVector2D(-120, -125);
	Offsets[EShooterHudPosition::Back] = FVector2D(0, -173);
	Offsets[EShooterHudPosition::BackLeft] = FVector2D(120, -125);

	Offset = 20.0f;
	HUDLight = FColor(50, 250, 50, 192);
	HUDDark = FColor(110, 124, 131, 255);
	//ShadowedFont.bEnableShadow = true;
	PlayState = EPlayState::EPlaying;
}

void AGamePlayHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGamePlayHUD::NotifyOutOfAmmo()
{
	NoAmmoNotifyTime = GetWorld()->GetTimeSeconds();
}

void AGamePlayHUD::SetPlayState(EPlayState NewPlayState)
{
	PlayState = NewPlayState;
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
				float CenterX = Canvas->ClipX / 2;
				float CenterY = Canvas->ClipY / 2;

				Canvas->SetDrawColor(HUDLight);

				FCanvasIcon* CurrentCrosshair[5];

				for (int32 i = 0; i < 5; i++)
				{
					// in Case you don't use custome cross hair for the weapon
					CurrentCrosshair[i] = &Crosshair[i];
				}
				// Start the drawing 
				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Center],
					CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).UL * ScaleUI / 2.0f,
					CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).VL * ScaleUI / 2.0f, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Bottom],
					CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Bottom]).UL * ScaleUI / 2.0f,
					CenterY + CrossSpread * ScaleUI, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Left],
					CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Left]).UL * ScaleUI - CrossSpread * ScaleUI,
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

void AGamePlayHUD::DrawHitIndicator()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastHitTime <= HitNotifyDisplayTime)
	{
		const float StartX = Canvas->ClipX / 2.0f;
		const float StartY = Canvas->ClipY / 2.0f;

		for (uint8 i = 0; i < 8; i++)
		{
			//const float TimeModifier = FMath::Max(0.0f, 1 - (CurrentTime - HitNotifyData[i].HitTime) / HitNotifyDisplayTime);
			const float TimeModifier = FMath::Max(0.0f, 1 - (CurrentTime - LastHitTime) / HitNotifyDisplayTime);
			const float Alpha = TimeModifier * HitNotifyData[i].HitPercentage;
			int x = FMath::Clamp(FMath::TruncToInt(Alpha * 255 * 10), 0, 255);
			Canvas->SetDrawColor(255, 255, 255, FMath::Clamp(FMath::TruncToInt(TimeModifier * 255 ), 0, 255));
			Canvas->DrawIcon(HitNotifyIcon[i],
				StartX + (HitNotifyIcon[i].U - HitNotifyTexture->GetSizeX() / 2 + Offsets[i].X) * ScaleUI,
				StartY + (HitNotifyIcon[i].V - HitNotifyTexture->GetSizeY() / 2 + Offsets[i].Y) * ScaleUI,
				ScaleUI);
		}


	}

}

void AGamePlayHUD::DrawKills()
{
	AGamePlayPlayerController* MyPC = Cast<AGamePlayPlayerController>(PlayerOwner);
	AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(MyPC->PlayerState);

	if (MyPlayerState)
	{
		// Draw BackGround and kills icons
		Canvas->SetDrawColor(HUDLight);
		const float KillsPosX = Canvas->OrgX + Offset * ScaleUI + 200;
		const float KillsPosY = Canvas->OrgY + Offset * ScaleUI + 200;
		Canvas->DrawIcon(KillsBg, KillsPosX, KillsPosY, ScaleUI);

		Canvas->DrawIcon(KillsIcon, KillsPosX + Offset * ScaleUI, KillsPosY + ((KillsBg.VL - KillsIcon.VL) / 2) * ScaleUI, ScaleUI);

		// Draw the text of the number of kills
		float TextScale = 0.57f;
		float SizeX, SizeY;
		FString Text = "KILLS:";
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);

		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDLight);
		//TextItem.EnableShadow(FLinearColor::Black);
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
		//TextItem.FontRenderInfo = ShadowedFont;
		//TextItem.SetColor(HUDDark);

		Canvas->DrawItem(
			TextItem,
			KillsPosX + Offset * ScaleUI + KillsIcon.UL * 1.5f * ScaleUI,
			KillsPosY + (KillsBg.VL * ScaleUI - SizeY * TextScale * ScaleUI) / 2
		);

		// Draw the Number
		Text = FString::FromInt(MyPlayerState->GetKills());

		TextScale = 0.88f;
		float BoxWidth = 135.0f * ScaleUI;
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
		Canvas->DrawItem(
			TextItem, KillsPosX + KillsBg.UL * ScaleUI - (BoxWidth + SizeX * TextScale * ScaleUI) / 2,
			KillsPosY + (KillsBg.VL* ScaleUI - SizeY * TextScale * ScaleUI) / 2
		);

	}
}

void AGamePlayHUD::DrawHealth()
{
	AGamePlayCharacter* MyPawn = Cast<AGamePlayCharacter>(GetOwningPawn());
	if (MyPawn)
	{
		Canvas->SetDrawColor(HUDLight);

		const float HealthPosX = (Canvas->ClipX - HealthBarBg.UL * ScaleUI) / 2;
		const float HealthPosY = Canvas->ClipY - (Offset + HealthBarBg.VL) * ScaleUI - 200;
		Canvas->DrawIcon(HealthBarBg, HealthPosX, HealthPosY, ScaleUI);

		const float HealthAmount = FMath::Min(1.0f, MyPawn->GetHealth() / MyPawn->GetMaxHealth());

		FCanvasTileItem TileItem(FVector2D(HealthPosX, HealthPosY), HealthBar.Texture->Resource,
			FVector2D(HealthBar.UL * HealthAmount  * ScaleUI, HealthBar.VL * ScaleUI), FLinearColor::White);

		MakeUV(HealthBar, TileItem.UV0, TileItem.UV1, HealthBar.U, HealthBar.V, HealthBar.UL * HealthAmount, HealthBar.VL);

		TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;

		Canvas->DrawItem(TileItem);

		Canvas->DrawIcon(HealthIcon, HealthPosX + Offset * ScaleUI, HealthPosY + (HealthBar.VL - HealthIcon.VL) / 2.0f * ScaleUI, ScaleUI);
	}

}

void AGamePlayHUD::NotifyGotHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	//AGamePlayCharacter* MyPawn = (PlayerOwner) ? Cast<AGamePlayCharacter>(PlayerOwner->GetPawn()) : NULL;
	//if (MyPawn)
	//{
	//	if (CurrentTime - LastHitTime > HitNotifyDisplayTime)
	//	{
	//		for (uint8 i = 0; i < 8; i++)
	//		{
	//			HitNotifyData[i].HitPercentage = 0;
	//		}
	//	}

	//	FVector ImpulseDir;
	//	FHitResult Hit;
	//	DamageEvent.GetBestHitInfo(this, PawnInstigator, Hit, ImpulseDir);

	//	//check hit vector against pre-defined direction vectors - left, front, right, back
	//	const FVector HitVector = FRotationMatrix(PlayerOwner->GetControlRotation()).InverseTransformVector(-ImpulseDir);

	//	FVector Dirs2[8] = {
	//		FVector(0,-1,0) /*left*/,
	//		FVector(1,-1,0) /*front left*/,
	//		FVector(1,0,0) /*front*/,
	//		FVector(1,1,0) /*front right*/,
	//		FVector(0,1,0) /*right*/,
	//		FVector(-1,1,0) /*back right*/,
	//		FVector(-1,0,0), /*back*/
	//		FVector(-1,-1,0) /*back left*/
	//	};
	//	int32 DirIndex = -1;
	//	float HighestModifier = 0;

	//	for (uint8 i = 0; i < 8; i++)
	//	{
	//		//Normalize our direction vectors
	//		Dirs2[i].Normalize();
	//		const float DirModifier = FMath::Max(0.0f, FVector::DotProduct(Dirs2[i], HitVector));
	//		if (DirModifier > HighestModifier)
	//		{
	//			DirIndex = i;
	//			HighestModifier = DirModifier;
	//		}
	//	}
	//	if (DirIndex > -1)
	//	{
	//		const float DamageTakenPercentage = (DamageTaken / MyPawn->GetHealth());
	//		HitNotifyData[DirIndex].HitPercentage += DamageTakenPercentage * 2;
	//		HitNotifyData[DirIndex].HitPercentage = FMath::Clamp(HitNotifyData[DirIndex].HitPercentage, 0.0f, 1.0f);
	//		HitNotifyData[DirIndex].HitTime = CurrentTime;
	//	}

	//}

	LastHitTime = CurrentTime;
}

void AGamePlayHUD::MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL)
{
	if (Icon.Texture)
	{
		const float Width = Icon.Texture->GetSurfaceWidth();
		const float Height = Icon.Texture->GetSurfaceHeight();
		UV0 = FVector2D(U / Width, V / Height);
		UV1 = UV0 + FVector2D(UL / Width, VL / Height);
	}
}

void AGamePlayHUD::DrawEndOfGameScreen()
{
	AGamePlayPlayerController* MyPC = Cast<AGamePlayPlayerController>(PlayerOwner);
	AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(MyPC->PlayerState);

	if (MyPlayerState)
	{
		Canvas->SetDrawColor(HUDLight);
		const float CenterX = Canvas->ClipX / 2;
		const float CenterY = Canvas->ClipY / 2;

		// Draw the Headline Text
		float TextScale = 1.0f;
		float SizeX, SizeY;
		FString Text = PlayState == EPlayState::EWon? "Ma3alem" : "Batee5a";
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);

		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDLight);
		//TextItem.EnableShadow(FLinearColor::Black);
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
		//TextItem.FontRenderInfo = ShadowedFont;
		//TextItem.SetColor(HUDDark);

		Canvas->DrawItem(
			TextItem,
			CenterX - SizeX * TextScale * ScaleUI / 2,
			CenterY - SizeY * TextScale * ScaleUI - Offset * ScaleUI
		);

		// Draw the Number
		Text = "Score: " + FString::FromInt(MyPlayerState->GetKills());

		TextScale = 0.7f;
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
		Canvas->DrawItem(
			TextItem,
			CenterX - SizeX * TextScale * ScaleUI / 2,
			CenterY + SizeY * TextScale * ScaleUI + Offset * ScaleUI
		);


	}
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

	if (PlayState == EPlayState::EPlaying)
	{
		DrawHealth();
		DrawCrosshair();
		DrawHitIndicator();
		DrawKills();
	}
	else
	{
		DrawEndOfGameScreen();
	}

}

