// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
	AZombieAIController();

	/* Called whenever the controller possesses a character bot */
	virtual void Possess(APawn* InPawn) override;
	
	virtual void UnPossess() override;
	
	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName CurrentWaypointKeyName;

public:
	ATargetPoint* GetWaypoint();

	ACharacter* GetTargetEnemy();


	void SetWaypoint(ATargetPoint* NewWaypoint);

	void SetTargetEnemy(APawn* NewTarget);

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
