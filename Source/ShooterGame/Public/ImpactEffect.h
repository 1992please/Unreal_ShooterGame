// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ImpactEffect.generated.h"

UCLASS()
class SHOOTERGAME_API AImpactEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImpactEffect();

	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;

	
	UPROPERTY(EditDefaultsOnly, Category= "ParticleEffects")
	UParticleSystem* DefaultFX;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundCue* DefaultSound;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterial* DefaultDecalMat;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalLifeSpan;

	UPROPERTY(EditDefaultsOnly, Category = "ParticleEffects")
	UParticleSystem* MetalFX;

	UPROPERTY(EditDefaultsOnly, Category = "ParticleEffects")
	UParticleSystem* FleshFX;

	UPROPERTY(EditDefaultsOnly, Category = "ParticleEffects")
	UParticleSystem* GlassFX;

	UPROPERTY(EditDefaultsOnly, Category = "ParticleEffects")
	UParticleSystem* ConcreteFX;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundCue* MetalSound;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundCue* FleshSound;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundCue* GlassSound;

	UPROPERTY(EditDefaultsOnly, Category = "SoundEffects")
	USoundCue* ConcreteSound;

	FHitResult HitResult;

	FRotator RandomDecalRotation;

};
