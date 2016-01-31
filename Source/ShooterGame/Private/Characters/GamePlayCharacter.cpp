// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayCharacter.h"
#include "ShooterGameInstance.h"
#include "Weapon.h"

// Sets default values
AGamePlayCharacter::AGamePlayCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	WeaponPullDownPercent = 0;

	//Set Mesh Variables
	GetMesh()->RelativeLocation.Z = -86.f;
	GetMesh()->RelativeRotation.Yaw = -90.f;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->bCastHiddenShadow = true;

	//Set Camera Variables
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->AttachTo(RootComponent);
	FollowCamera->RelativeLocation = FVector(0, 0, 75.99f);
	FollowCamera->FieldOfView = 114.f;
	FollowCamera->bUsePawnControlRotation = true;

	//FPP Mesh
	FPPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPPMesh"));
	FPPMesh->AttachParent = FollowCamera;
	FPPMesh->RelativeLocation = FVector(-5.769128f, 20.0f, -144.999741f);
	FPPMesh->RelativeRotation = FRotator(0, -90, 0);
	FPPMesh->bOnlyOwnerSee = true;
	FPPMesh->CastShadow = false;

	//SetTimeLine
	static const ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Data/ChangeWeaponCurve.ChangeWeaponCurve'"));
	Timeline = FTimeline();
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName(TEXT("TimeLineChangeProgress")));
	Timeline.AddInterpFloat(Curve.Object, ProgressFunction);

	FOnTimelineEvent WeaponDownEvent;
	WeaponDownEvent.BindUFunction(this, TEXT("TimeLineWeaponDown"));
	Timeline.AddEvent(.5f, WeaponDownEvent);
}

// Called when the game starts or when spawned
void AGamePlayCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeaponsAndAssignToSlots();
	//EquipWeapon(WeaponSlot1);
}

// Called every frame
void AGamePlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bisChangingWeapon)
	{
		if (Timeline.IsPlaying())
		{
			Timeline.TickTimeline(DeltaTime);
		}
		else
		{
			bisChangingWeapon = false;
		}
	}

}

// Called to bind functionality to input
void AGamePlayCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AGamePlayCharacter::OnStartFire);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AGamePlayCharacter::OnStopFire);

	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AGamePlayCharacter::OnReload);
}

void AGamePlayCharacter::OnStartFire()
{
	if (CurrentWeapon)
		CurrentWeapon->StartFire();
}

void AGamePlayCharacter::OnStopFire()
{
	if (CurrentWeapon)
		CurrentWeapon->StopFire();
}

void AGamePlayCharacter::OnReload()
{
	if(CurrentWeapon)
		CurrentWeapon->StartReload();
}

UShooterGameInstance* AGamePlayCharacter::GetShooterGameInstance()
{
	return Cast<UShooterGameInstance>(GetGameInstance());
}

void AGamePlayCharacter::SpawnWeaponsAndAssignToSlots()
{
	const TArray<FWeaponBackpackItem>& WeaponBackpackItems = GetShooterGameInstance()->Backpack_Weapons;
	UWorld* const World = GetWorld();
	if (World)
	{
		for (int index = 0; index < WeaponBackpackItems.Num(); index++)
		{
			switch (WeaponBackpackItems[index].InSlot)
			{
				case 1:
				{
					WeaponSlot1 = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponBackpackItems[index].WeaponToSpawn, GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					if (WeaponSlot1)
					{
						//WeaponSlot1->IndexInBackpack = index;
						WeaponSlot1->SetOwningPawn(this);
						UGameplayStatics::FinishSpawningActor(WeaponSlot1, GetTransform());
						WeaponSlot1->AttachRootComponentTo(FPPMesh, WeaponSlot1->AttachSocketNameFPP, EAttachLocation::SnapToTargetIncludingScale);
					}
					break;
				}
				case 2:
				{
					WeaponSlot2 = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponBackpackItems[index].WeaponToSpawn, GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					if (WeaponSlot2)
					{
						WeaponSlot2->SetOwningPawn(this);
						//WeaponSlot2->IndexInBackpack = index;
						UGameplayStatics::FinishSpawningActor(WeaponSlot2, GetTransform());
						WeaponSlot2->AttachRootComponentTo(FPPMesh, WeaponSlot2->AttachSocketNameFPP, EAttachLocation::SnapToTargetIncludingScale);
					}
					break;
				}
				case 3:
				{
					WeaponSlot3 = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponBackpackItems[index].WeaponToSpawn, GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					if (WeaponSlot3)
					{
						WeaponSlot3->SetOwningPawn(this);

						//WeaponSlot3->IndexInBackpack = index;
						UGameplayStatics::FinishSpawningActor(WeaponSlot3, GetTransform());
						WeaponSlot3->AttachRootComponentTo(FPPMesh, WeaponSlot3->AttachSocketNameFPP, EAttachLocation::SnapToTargetIncludingScale);
					}
					break;
				}
			}
		}
	}
	// Set Current Weapon
	ShowWeapon(WeaponSlot1);
	CurrentWeapon = WeaponSlot1;

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "SpawnWeaponsAndAssignToSlots");
}

void AGamePlayCharacter::ShowWeapon(AWeapon* WeaponToShow)
{
	WeaponSlot1->SetActorHiddenInGame(!(WeaponToShow == WeaponSlot1));
	WeaponSlot2->SetActorHiddenInGame(!(WeaponToShow == WeaponSlot2));
	WeaponSlot3->SetActorHiddenInGame(!(WeaponToShow == WeaponSlot3));
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Show Weapon");
}

void AGamePlayCharacter::EquipWeapon(AWeapon* WhichWeapon)
{
	if (WhichWeapon == CurrentWeapon || bisChangingWeapon)
		return;

	bisChangingWeapon = true;

	BindingToBeEquiped = WhichWeapon;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Equipe Weapon");
	Timeline.PlayFromStart();
}

void AGamePlayCharacter::TimeLineChangeProgress(float Value)
{
	WeaponPullDownPercent = Value;
}

void AGamePlayCharacter::TimeLineWeaponDown()
{
	// We can show weapon when hands are down
	ShowWeapon(BindingToBeEquiped);


	// Set current weapon after equipping
	CurrentWeapon = BindingToBeEquiped;
}

float AGamePlayCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	if (FPPMesh && AnimMontage && FPPMesh->AnimScriptInstance)
	{
		return FPPMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}
	return 0.0f;
}

void AGamePlayCharacter::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	if (FPPMesh && AnimMontage && FPPMesh->AnimScriptInstance && FPPMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		FPPMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOutTime);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Stoping anim montage");
}

void AGamePlayCharacter::StopAllAnimMontages()
{
	if (FPPMesh && FPPMesh->AnimScriptInstance)
	{
		FPPMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}
}