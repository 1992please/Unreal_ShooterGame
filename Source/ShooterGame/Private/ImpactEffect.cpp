// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ImpactEffect.h"


// Sets default values
AImpactEffect::AImpactEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Default Values
	DecalSize = 2;
	DecalLifeSpan = 3;
}

// Called when the game starts or when spawned
void AImpactEffect::BeginPlay()
{
	Super::BeginPlay();

	// rotate the decal in the direction of the normal vector of the HitResult with random roll rotation
	RandomDecalRotation = HitResult.Normal.Rotation() + FRotator(0, 0, FMath::RandRange(-180, 180));

	UWorld* World = GetWorld();
	if (World && HitResult.PhysMaterial.IsValid())
	{
		switch (HitResult.PhysMaterial->SurfaceType)
		{
			case EPhysicalSurface::SurfaceType_Default:
			{
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
			case EPhysicalSurface::SurfaceType1:
			{
				break;
			}
			case EPhysicalSurface::SurfaceType2:
			{
				break;
			}
			case EPhysicalSurface::SurfaceType3:
			{
				break;
			}

		}
	}
}

// Called every frame
void AImpactEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

