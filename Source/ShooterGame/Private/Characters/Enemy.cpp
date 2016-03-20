// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Enemy.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MaxHealth = 100;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

bool AEnemy::HealthToModify(float HealthToModify)
{
	CurrentHealth = FMath::Max(0.0f, CurrentHealth - HealthToModify);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::SanitizeFloat(CurrentHealth));

	return !CurrentHealth;
}

void AEnemy::CalculateDamage(float Damage, float CritChance, float CritDamageModifier, FHitResult& HitInfo, float& OutDamageTaken, bool& OutWasCrit)
{
	if (FMath::FRand() < CritChance)
	{
		OutDamageTaken = CritDamageModifier * Damage;
		OutWasCrit = true;
	}
	else
	{
		OutDamageTaken = Damage;
		OutWasCrit = false;
	}

	if (HitInfo.PhysMaterial.IsValid())
	{
		OutDamageTaken *= HitInfo.PhysMaterial->DestructibleDamageThresholdScale;
	}
}


