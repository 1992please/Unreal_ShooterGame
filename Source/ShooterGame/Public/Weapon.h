// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "STypes.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

namespace EWeaponState
{
	enum Type
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
	};
}

UCLASS()
class SHOOTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()

	/** perform initial setup */
	virtual void PostInitializeComponents() override;

public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = UI)
	uint8 IndexInBackpack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponAttribute)
	FName AttachSocketNameFPP;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = WeaponAttribute)
	FName AttachSocketNameTPP;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = WeaponAttribute)
	EWeaponType WeaponType;

public:
	/*
	* How much ammo we have in the magazine
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= WeaponAttribute)
	int32 CurrentAmmoInClip;

	/**
	* How much overal Ammo
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = WeaponAttribute)
	int32 CurrentAmmo;

public:
	virtual void StartFire();

	virtual void StopFire();

	virtual void StartReload();

	virtual void StopReload();

	virtual void ReloadWeapon();

	void SetOwningPawn(class AGamePlayCharacter* NewOwner);
protected:
/** weapon data */
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;
	
	bool bWantsToFire;

	bool bPendingReload;

	EWeaponState::Type CurrentState;

	float LastFireTime;
	/** Handle for efficient management of HandleFiring timer */
	FTimerHandle TimerHandle_HandleFiring;

	/** Handle the Stoping the reload */
	FTimerHandle TimerHandle_StopReload;

	/** Handle The Change in the Clip Ammo while reloading */
	FTimerHandle TimerHandle_ReloadWeapon;

	AGamePlayCharacter* MyPawn;

	/** is muzzle FX looped? */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	uint32 bLoopedMuzzleFX : 1;

	/** firing audio (bLoopedFireSound set) */
	UPROPERTY(Transient)
	UAudioComponent* FireAC;

	/** name of bone/socket for muzzle in weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName MuzzleAttachPoint;

	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleFX;

	/** is fire animation looped? */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	uint32 bLoopedFireAnim : 1;

	/** is fire animation playing? */
	uint32 bPlayingFireAnim : 1;

	/** is fire sound looped? */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	uint32 bLoopedFireSound : 1;

	/** Weapopn is Refiring */
	uint32 bRefiring;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* PawnAnim;

	/** single fire sound (bLoopedFireSound not set) */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* FireSound;

	/** looped fire sound (bLoopedFireSound set) */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* FireLoopSound;

	/** finished burst sound (bLoopedFireSound set) */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* FireFinishSound;

	/** reload sound */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* ReloadSound;

	/** reload animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* ReloadAnim;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	float SpreadMin;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	float SpreadMax;

	float SpreadCurrent;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	FAmmoData AmmoData;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	UParticleSystem* TrailFX;


	void DetermineWeaponState();

	void SetWeaponState(EWeaponState::Type NewState);

	void OnBurstStarted();

	void OnBurstFinished();

	void HandleFiring();

	void SimulateWeaponFire();

	void UseAmmo();

	bool CanReload() const;

	bool CanFire() const;

public:
	void CalculateShootInformations(UCameraComponent* Camera, USceneComponent* WeaponMesh, FName WeaponFireSocketName, FTransform& Transform, FHitResult& HitResult, FVector& EndLocation);
};
