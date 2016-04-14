// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ZombieAIController.h"
#include "Enemy.h"

/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AZombieAIController::AZombieAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	/* Match with the AI/ZombieBlackboard */
	PatrolLocationKeyName = "PatrolLocation";
	CurrentWaypointKeyName = "CurrentWaypoint";
	BotTypeKeyName = "BotType";
	TargetEnemyKeyName = "TargetEnemy";


}

void AZombieAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(InPawn);
	if (Enemy)
	{
		if (Enemy->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Enemy->BehaviorTree->BlackboardAsset);
		}
		BehaviorComp->StartTree(*Enemy->BehaviorTree);
	}
}

void AZombieAIController::UnPossess()
{
	Super::UnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
}

void AZombieAIController::SetWaypoint(ATargetPoint* NewWaypoint)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(CurrentWaypointKeyName, NewWaypoint);
	}
}

void AZombieAIController::SetTargetEnemy(APawn* NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

ATargetPoint* AZombieAIController::GetWaypoint()
{
	if (BlackboardComp)
	{
		return Cast<ATargetPoint>(BlackboardComp->GetValueAsObject(CurrentWaypointKeyName));
	}

	return nullptr;
}

ACharacter* AZombieAIController::GetTargetEnemy()
{
	if (BlackboardComp)
	{
		return Cast<ACharacter>(BlackboardComp->GetValueAsObject(TargetEnemyKeyName));
	}

	return nullptr;
}