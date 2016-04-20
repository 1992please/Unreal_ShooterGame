// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "EnemyZombie.h"
#include "ShooterPlayerState.h"
#include "ZombieAIController.h"

AEnemyZombie::AEnemyZombie()
{
	/* Note: We assign the Controller class in the Blueprint extension of this class
	Because the zombie AIController is a blueprint in content and it's better to avoid content references in code.  */
	/*AIControllerClass = ASZombieAIController::StaticClass();*/
	PrimaryActorTick.bCanEverTick = false;
	/* Ignore this channel or it will absorb the trace impacts instead of the skeletal mesh */
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);

	/* These values are matched up to the CapsuleComponent above and are used to find navigation paths */
	GetMovementComponent()->NavAgentProps.AgentRadius = 42;
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;

	// Set Some Attributes for MeleeCollision.
	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->SetRelativeLocation(FVector(45, 0, 25));
	MeleeCollisionComp->SetCapsuleHalfHeight(60);
	MeleeCollisionComp->SetCapsuleRadius(35, false);
	MeleeCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeCollisionComp->AttachParent = GetCapsuleComponent();

	// Zombie Audio Loop
	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->AttachParent = RootComponent;

	Health = 100;
	MeleeDamage = 24.0f;
	MeleeStrikeCooldown = 1.0f;
}

void AEnemyZombie::BeginPlay()
{
	Super::BeginPlay();

	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyZombie::OnMeleeCompBeginOverlap);
	}

	PlaySoundLoop();

	/* Assign a basic name to identify the bots in the HUD. */
	AShooterPlayerState* PS = Cast<AShooterPlayerState>(PlayerState);
	if (PS)
	{
		PS->SetPlayerName("Bot");
		PS->bIsABot = true;
	}

	// Start Making the zombie chase the player
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			OnSeePlayer(Pawn);
		}
	}

}

void AEnemyZombie::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AEnemyZombie::OnSeePlayer(APawn* Pawn)
{
	/* Keep track of the time the player was last sensed in order to clear the target */

	AZombieAIController* AIController = Cast<AZombieAIController>(GetController());
	ABaseCharacter* SensedPawn = Cast<ABaseCharacter>(Pawn);
	if (AIController && SensedPawn->IsAlive())
	{
		AIController->SetTargetEnemy(SensedPawn);
	}
}

void AEnemyZombie::PlaySoundLoop()
{
	AudioLoopComp->SetSound(SoundHunting);
	AudioLoopComp->Play();
}

void AEnemyZombie::PerformMeleeStrike(AActor* HitActor)
{
	if (LastMeleeAttackTime + MeleeStrikeCooldown > GetWorld()->GetTimeSeconds())
	{
		/* Set timer to start attacking as soon as the cooldown elapses. */
		if (!TimerHandle_MeleeAttack.IsValid())
		{
			// TODO: Set Timer
		}

		/* Attacked before cooldown expired */
		return;
	}

	if (HitActor && HitActor != this && IsAlive() && PunchDamageType)
	{
		ACharacter* OtherPawn = Cast<ACharacter>(HitActor);
		if (OtherPawn)
		{
			AShooterPlayerState* MyPS = Cast<AShooterPlayerState>(PlayerState);
			AShooterPlayerState* OtherPS = Cast<AShooterPlayerState>(OtherPawn->PlayerState);

			if (MyPS && OtherPS)
			{
				// TODO Use TeamNumbers to identify Allies

				//if (MyPS->GetTeamNumber() == OtherPS->GetTeamNumber())
				if(!OtherPS->bIsABot)
				{
					/* Do not attack other zombies. */
					return;
				}

				/* Set to prevent a zombie to attack multiple times in a very short time */
				LastMeleeAttackTime = GetWorld()->GetTimeSeconds();

				FPointDamageEvent DmgEvent;
				DmgEvent.DamageTypeClass = PunchDamageType;
				DmgEvent.Damage = MeleeDamage;

				HitActor->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);

				SimulateMeleeStrike();
			}
		}
	}
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
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Attack");
	/* Stop any running attack timers */
	TimerHandle_MeleeAttack.Invalidate();

	PerformMeleeStrike(OtherActor);

	/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &AEnemyZombie::OnRetriggerMeleeStrike, MeleeStrikeCooldown, true);
}

void AEnemyZombie::OnRetriggerMeleeStrike()
{
	/* Apply damage to a single random pawn in range. */
	TArray<AActor*> Overlaps;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ABaseCharacter::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ABaseCharacter* OverlappingPawn = Cast<ABaseCharacter>(Overlaps[i]);
		if (OverlappingPawn)
		{
			PerformMeleeStrike(OverlappingPawn);
			//break; /* Uncomment to only attack one pawn maximum */
		}
	}

	/* No pawns in range, cancel the retrigger timer */
	if (Overlaps.Num() == 0)
	{
		TimerHandle_MeleeAttack.Invalidate();
	}
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
