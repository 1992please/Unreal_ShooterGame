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
	PrimaryActorTick.bCanEverTick = true;
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
	StartUpTime = .01f;
	SneakySpeed = 50;
	ChargeSpeed = 100;
	bRagdolledAfterDeath = true;
	bRagdolledAfterDeath = false;
}

void AEnemyZombie::BeginPlay()
{
	Super::BeginPlay();

	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyZombie::OnMeleeCompBeginOverlap);
	}

	PlaySoundLoop(SoundSneak);

	/* Assign a basic name to identify the bots in the HUD. */
	AShooterPlayerState* PS = Cast<AShooterPlayerState>(PlayerState);
	if (PS)
	{
		PS->SetPlayerName("Bot");
		PS->bIsABot = true;
	}

	GetCharacterMovement()->MaxWalkSpeed = SneakySpeed;

	GetMovementComponent()->NavAgentProps.AgentRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	GetMovementComponent()->NavAgentProps.AgentHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2;
}

void AEnemyZombie::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetCharacter && (CharacterMovement->MaxWalkSpeed != ChargeSpeed))
	{
		const FVector MyLocation = GetActorLocation();
		const FVector TargetLocation = TargetCharacter->GetActorLocation();
		if ((TargetLocation - MyLocation).Size() < DistanceBeforeCharging)
		{
			CharacterMovement->MaxWalkSpeed = ChargeSpeed;
			PlaySoundLoop(SoundCharge);
		}
	}
}

void AEnemyZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Start Making the zombie chase the player
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyZombie::StartAttackingPlayer, StartUpTime, false);
}

void AEnemyZombie::StartAttackingPlayer()
{
	/* Keep track of the time the player was last sensed in order to clear the target */

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			AZombieAIController* AIController = Cast<AZombieAIController>(GetController());
			TargetCharacter = Cast<ABaseCharacter>(Pawn);
			if (AIController && TargetCharacter->IsAlive())
			{
				AIController->SetTargetEnemy(TargetCharacter);
			}
		}
	}
}

void AEnemyZombie::PlaySoundLoop(USoundCue* LoopSound)
{
	AudioLoopComp->SetSound(SoundCharge);
	AudioLoopComp->Play();
}

void AEnemyZombie::PerformMeleeStrike(AActor* HitActor)
{
	if (IsDying())
	{
		return;
	}

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
				if(OtherPS->bIsABot)
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
	PlayAnimMontage(MeleeAnimMontages[FMath::RandRange(0, MeleeAnimMontages.Num() - 1)]);
	PlayCharacterSound(SoundAttackMelee);
}


void AEnemyZombie::OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
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


void AEnemyZombie::PlayHit(bool bKilled, float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	Super::PlayHit(bKilled, DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	/* Stop playing the hunting sound */
	if (AudioLoopComp && bKilled)
	{
		AudioLoopComp->Stop();
	}
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		UPhysicalMaterial * PhysMat = PointDamageEvent->HitInfo.PhysMaterial.Get();
		if (PhysMat)
		{
			if (HeadHitAnimMontage && PhysMat->SurfaceType == SURFACE_HEAD)
			{
				PlayAnimMontage(HeadHitAnimMontage);
			}
			else if (BodyHitAnimMontage && PhysMat->SurfaceType == SURFACE_BODY)
			{
				PlayAnimMontage(BodyHitAnimMontage);
			}
			else if(DefaultHitAnimMontage)
			{
				PlayAnimMontage(DefaultHitAnimMontage);
			}
		}
	}
}