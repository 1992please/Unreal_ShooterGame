// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Enemy.h"
#include "ShooterDamageType.h"

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

float AEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (CurrentHealth <= 0.f)
	{
		return 0.f;
	}

	/* Modify based based on gametype rules */
	AGamePlayGameMode* MyGameMode = Cast<AGamePlayGameMode>(GetWorld()->GetAuthGameMode());
	//Damage = MyGameMode ? MyGameMode->ModifyDamage(Damage, this, DamageEvent, EventInstigator, DamageCauser) : Damage;

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		CurrentHealth -= ActualDamage;
		if (CurrentHealth <= 0)
		{
			bool bCanDie = true;

			/* Check the damagetype, always allow dying if the cast fails, otherwise check the property if player can die from damagetype */
			if (DamageEvent.DamageTypeClass)
			{
				UShooterDamageType* DmgType = Cast<UShooterDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
				bCanDie = (DmgType == nullptr || (DmgType && DmgType->GetCanDieFrom()));
			}

			if (bCanDie)
			{
				//Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
			}
			else
			{
				/* Player cannot die from this damage type, set hitpoints to 1.0 */
				CurrentHealth = 1.0f;
			}
		}
		else
		{
			/* Shorthand for - if x != null pick1 else pick2 */
			APawn* Pawn = EventInstigator ? EventInstigator->GetPawn() : nullptr;
			//PlayHit(ActualDamage, DamageEvent, Pawn, DamageCauser, false);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::SanitizeFloat(CurrentHealth));

	return ActualDamage;
}
