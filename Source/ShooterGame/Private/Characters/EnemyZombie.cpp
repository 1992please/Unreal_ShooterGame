// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "EnemyZombie.h"



AEnemyZombie::AEnemyZombie()
{
	/* Note: We assign the Controller class in the Blueprint extension of this class
	Because the zombie AIController is a blueprint in content and it's better to avoid content references in code.  */
	/*AIControllerClass = ASZombieAIController::StaticClass();*/

	/* Ignore this channel or it will absorb the trace impacts instead of the skeletal mesh */
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);


}
void AEnemyZombie::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyZombie::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AEnemyZombie::PerformMeleeStrike(AActor* HitActor)
{

}

UAudioComponent* AEnemyZombie::PlayCharacterSound(USoundCue* CueToPlay)
{
	if (CueToPlay)
	{
		return UGameplayStatics::SpawnSoundAttached(CueToPlay, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
	}

	return nullptr;
}

void AEnemyZombie::SimulateMeleeStrike()
{
	PlayAnimMontage(MeleeAnimMontage);
	PlayCharacterSound(SoundAttackMelee);
}


void AEnemyZombie::OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void AEnemyZombie::OnRetriggerMeleeStrike()
{
}


void AEnemyZombie::PlayHit(bool bKilled)
{
	Super::PlayHit(bKilled);

	/* Stop playing the hunting sound */
	if (AudioLoopComp && bKilled)
	{
		AudioLoopComp->Stop();
	}
}
