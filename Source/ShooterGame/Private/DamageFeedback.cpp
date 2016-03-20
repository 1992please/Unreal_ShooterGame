// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "DamageFeedback.h"

// Sets default values
ADamageFeedback::ADamageFeedback()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADamageFeedback::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageFeedback::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

