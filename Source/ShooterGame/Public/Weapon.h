// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "STypes.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SHOOTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
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
	int32 CurrentAmmoInMag;

	// Max ammo in mag
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = WeaponAttribute)
	int32 MaxAmmoInMag;

	/**
	* How much ammo we have in Backpack
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = WeaponAttribute)
	int32 CurrentAmmoInBackpack;

	// max ammo in Backpack
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = WeaponAttribute)
	int32 MaxAmmoInBackpack;

	// How needed for reload
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = WeaponAttribute)
	float ReloadTime;

public:
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Fire();
};
