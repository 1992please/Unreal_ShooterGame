// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ImpactEffect.h"


// Sets default values
AImpactEffect::AImpactEffect()
{

	bAutoDestroyWhenFinished = true;
	// Default Values
	DecalSize = 2;
	DecalLifeSpan = 3;
}

// Called when the game starts or when spawned
void AImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetLifeSpan(1);
	// rotate the decal in the direction of the normal vector of the HitResult with random roll rotation
	RandomDecalRotation = HitResult.Normal.Rotation();
	RandomDecalRotation.Roll = FMath::RandRange(-180.0f, 180.0f);

	UWorld* World = GetWorld();

	if (World && HitResult.PhysMaterial.IsValid())
	{
		switch (HitResult.PhysMaterial->SurfaceType)
		{
			case SURFACE_DEFAULT:
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Default");

				if (DefaultFX)
				{
					UGameplayStatics::SpawnEmitterAtLocation(World, DefaultFX, HitResult.ImpactPoint, RandomDecalRotation);
				}

				if (DefaultSound)
				{
					UGameplayStatics::SpawnSoundAtLocation(World, DefaultSound, HitResult.ImpactPoint);
				}

				if (DefaultDecalMat && HitResult.Component.IsValid())
				{
					UGameplayStatics::SpawnDecalAttached(DefaultDecalMat, FVector(DecalSize, DecalSize, 1.0f), HitResult.Component.Get(), HitResult.BoneName, HitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, DecalLifeSpan);
				}
				break;
			}
			case SURFACE_HEAD:
			case SURFACE_BODY:
			case SURFACE_LIMB:
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Flesh " +  HitResult.BoneName.ToString());
				break;
			}

		}
	}
}

