// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "BaseCharacter.h"
#include "ShooterDamageType.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Health = 100;
}

float ABaseCharacter::GetHealth() const
{
	return Health;
}

float ABaseCharacter::GetMaxHealth() const
{
	// Retrieve the default value of the health property that is assigned on instantiation.
	return GetClass()->GetDefaultObject<ABaseCharacter>()->Health;
}

bool ABaseCharacter::IsAlive() const
{
	return Health > 0;
}

float ABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (Health > 0.f)
	{
		/* Modify based based on gametype rules */
		//ASGameMode* MyGameMode = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		//Damage = MyGameMode ? MyGameMode->ModifyDamage(Damage, this, DamageEvent, EventInstigator, DamageCauser) : Damage;

		const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		if (ActualDamage > 0.f)
		{
			Health -= ActualDamage;
			if (Health <= 0)
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
					Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
				}
				else
				{
					/* Player cannot die from this damage type, set hitpoints to 1.0 */
					Health = 1.0f;
				}
			}
			else
			{
				/* Shorthand for - if x != null pick1 else pick2 */
				//APawn* Pawn = EventInstigator ? EventInstigator->GetPawn() : nullptr;
				PlayHit(false);
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::SanitizeFloat(Health));
		return ActualDamage;
	}
	else
	{
		ApplyPhysicsToTheRagdolledBody(DamageEvent);

	}
	return 0;
}

bool ABaseCharacter::CanDie() const
{
	/* Check if character is already dying, destroyed or if we have authority */
	if (bIsDying ||
		IsPendingKill() ||
		Role != ROLE_Authority ||
		GetWorld()->GetAuthGameMode() == NULL ||
		GetWorld()->GetAuthGameMode()->GetMatchState() == MatchState::LeavingMap)
	{
		return false;
	}

	return true;
}

void ABaseCharacter::FellOutOfWorld(const class UDamageType& DmgType)
{
	Die(Health, FDamageEvent(DmgType.GetClass()), NULL, NULL);
}

bool ABaseCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (!CanDie())
	{
		return false;
	}

	Health = FMath::Min(0.0f, Health);

	/* Fallback to default DamageType if none is specified */
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);

	/* Notify the gamemode we got killed for scoring and game over state */
	AController* KilledPlayer = Controller ? Controller : Cast<AController>(GetOwner());
	GetWorld()->GetAuthGameMode<AGamePlayGameMode>()->Killed(Killer, KilledPlayer, this, DamageType);

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);

	return true;
}

void ABaseCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}

	bReplicateMovement = false;
	bTearOff = true;
	bIsDying = true;

	PlayHit(true);

	DetachFromControllerPendingDestroy();

	/* Disable all collision on capsule */
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	USkeletalMeshComponent* Mesh3P = GetMesh();
	if (Mesh3P)
	{
		Mesh3P->SetCollisionProfileName(TEXT("Ragdoll"));
	}
	SetActorEnableCollision(true);

	SetRagdollPhysics();

	ApplyPhysicsToTheRagdolledBody(DamageEvent);
}

void ABaseCharacter::ApplyPhysicsToTheRagdolledBody(FDamageEvent const& DamageEvent)
{
	USkeletalMeshComponent* Mesh3P = GetMesh();
	if (Mesh3P)
	{
		/* Apply physics impulse on the bone of the enemy skeleton mesh we hit (ray-trace damage only) */
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			FPointDamageEvent PointDmg = *((FPointDamageEvent*)&DamageEvent);

			// TODO: Use DamageTypeClass->DamageImpulse
			Mesh3P->AddImpulseAtLocation(PointDmg.ShotDirection * 12000, PointDmg.HitInfo.ImpactPoint, PointDmg.HitInfo.BoneName);

		}
		if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
		{
			FRadialDamageEvent RadialDmg = *((FRadialDamageEvent const*)(&DamageEvent));

			Mesh3P->AddRadialImpulse(RadialDmg.Origin, RadialDmg.Params.GetMaxRadius(), 100000 /*RadialDmg.DamageTypeClass->DamageImpulse*/, ERadialImpulseFalloff::RIF_Linear);
		}
	}
}

void ABaseCharacter::PlayHit(bool bKilled)
{
	if (bKilled && SoundDeath)
	{
		UGameplayStatics::SpawnSoundAttached(SoundDeath, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
	}
	else if (SoundTakeHit)
	{
		UGameplayStatics::SpawnSoundAttached(SoundTakeHit, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
	}
}

void ABaseCharacter::SetRagdollPhysics()
{
	USkeletalMeshComponent* Mesh3P = GetMesh();

	if (!IsPendingKill() || Mesh3P || Mesh3P->GetPhysicsAsset())
	{
		Mesh3P->SetAllBodiesSimulatePhysics(true);
		Mesh3P->SetSimulatePhysics(true);
		Mesh3P->WakeAllRigidBodies();
		Mesh3P->bBlendPhysics = true;

		SetLifeSpan(10.0f);
	}
	else
	{
		// Immediately hide the pawn
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}
}