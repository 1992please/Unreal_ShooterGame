// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "WeaponRifle.h"
#include "GamePlayCharacter.h"
#include "ImpactEffect.h"
#include "ShooterGameInstance.h"

AWeaponRifle::AWeaponRifle()
{
	WeaponType = EWeaponType::Rifle;
	AttachSocketNameFPP = "S_Rifle";

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponObject(TEXT("SkeletalMesh'/Game/Weapons/Rifle/Rifle.Rifle'"));
	if (WeaponObject.Succeeded())
		WeaponMesh->SetSkeletalMesh(WeaponObject.Object);
}

void AWeaponRifle::BeginPlay()
{
	Super::BeginPlay();

	UShooterGameInstance*const GameInstance = Cast<UShooterGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		AmmoData = GameInstance->AmmoConventional;
	}

}

void AWeaponRifle::FireWeapon()
{
	UCameraComponent* Camera = MyPawn->GetCamera();

	if (WeaponMesh && Camera)
	{
		FHitResult HitResult;
		FTransform Transform;
		FVector StartLocation;
		CalculateShootInformations(Camera, WeaponMesh, MuzzleAttachPoint, Transform, HitResult, StartLocation);

		AddDamageTo(HitResult, StartLocation);

		UWorld* const World = GetWorld();
		if (World)
		{
			if (TrailFX)
			{
				UParticleSystemComponent* Trail = UGameplayStatics::SpawnEmitterAtLocation(World, TrailFX, WeaponMesh->GetSocketLocation(MuzzleAttachPoint));
				Trail->SetVectorParameter("ShockBeamEnd", HitResult.ImpactPoint);
			}

			if (HitResult.bBlockingHit && ImpactEffect)
			{
				FTransform const SpawnTransform(HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint);
				AImpactEffect* Impact = World->SpawnActorDeferred<AImpactEffect>(ImpactEffect, SpawnTransform);
				if (Impact)
				{
					Impact->HitResult = HitResult;
					UGameplayStatics::FinishSpawningActor(Impact, SpawnTransform);
				}
			}
		}
	}

	IncreaseSpread(0.1);
}