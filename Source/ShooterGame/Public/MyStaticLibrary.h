// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyStaticLibrary.generated.h"

/**
 *
 */
UCLASS()
class UMyStaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	static FORCEINLINE bool Trace(
		UWorld* World,
		AActor* ActorToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel = ECC_Pawn,
		bool ReturnPhysMat = false
		)
	{
		if (!World)
		{
			return false;
		}

		FCollisionQueryParams TraceParams(FName(TEXT("ShooterTrace")), true, ActorToIgnore);
		TraceParams.bTraceComplex = true;
		//TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

		//Ignore Actors
		TraceParams.AddIgnoredActor(ActorToIgnore);

		//Re-initialize hit info
		HitOut = FHitResult(ForceInit);

		//Trace!
		World->LineTraceSingleByChannel(
			HitOut,		//result
			Start,	//start
			End, //end
			CollisionChannel, //collision channel
			TraceParams
			);

		//Hit any Actor?
		return (HitOut.GetActor() != NULL);
	}

	//Trace with an Array of Actors to Ignore
	//   Ignore as many actors as you want!

	static FORCEINLINE bool Trace(
		UWorld* World,
		TArray<AActor*>& ActorsToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel = ECC_Pawn,
		bool ReturnPhysMat = false
		)
	{
		if (!World)
		{
			return false;
		}

		FCollisionQueryParams TraceParams(FName(TEXT("ShooterTrace")), true, ActorsToIgnore[0]);
		TraceParams.bTraceComplex = true;

		//TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

		//Ignore Actors
		TraceParams.AddIgnoredActors(ActorsToIgnore);

		//Re-initialize hit info
		HitOut = FHitResult(ForceInit);

		World->LineTraceSingleByChannel(
			HitOut,		//result
			Start,	//start
			End, //end
			CollisionChannel, //collision channel
			TraceParams
			);

		return (HitOut.GetActor() != NULL);
	}

	//Component-level trace, do a trace against just 1 component
	static FORCEINLINE bool TraceComponent(
		UPrimitiveComponent* TheComp,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut
		) {
		if (!TheComp) return false;
		if (!TheComp->IsValidLowLevel()) return false;
		//~~~~~~~~~~~~~~~~~~~~~

		FCollisionQueryParams TraceParams(FName(TEXT("ShooterCompTrace")), true, NULL);
		TraceParams.bTraceComplex = true;
		//TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = false;

		//Ignore Actors
		//TraceParams.AddIgnoredActors(ActorsToIgnore);

		//Re-initialize hit info
		HitOut = FHitResult(ForceInit);

		return TheComp->LineTraceComponent(
			HitOut,
			Start,
			End,
			TraceParams
			);
	}
};
