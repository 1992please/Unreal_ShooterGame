// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GamePlayCharacter.h"
#include "ShooterGameInstance.h"
#include "Weapon.h"
#include "GamePlayHUD.h"

// Sets default values
AGamePlayCharacter::AGamePlayCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	WeaponPullDownPercent = 0;

	LastTimeTakeDamage = 0;
	RegenerationRate = 5;
	RegenrationCastingTime = 3;

	//Set Mesh Variables
	USkeletalMeshComponent* Mesh = GetMesh();
	Mesh->RelativeLocation.Z = -86.f;
	Mesh->RelativeRotation.Yaw = -90.f;
	Mesh->bOwnerNoSee = true;
	Mesh->CastShadow = true;
	Mesh->bCastDynamicShadow = true;
	Mesh->bCastHiddenShadow = true;


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
}

// Called when the game starts or when spawned
void AGamePlayCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeaponsAndAssignToSlots();
}

// Called every frame
void AGamePlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health < GetMaxHealth() && RegenrationCastingTime > GetWorld()->TimeSeconds - LastTimeTakeDamage)
	{
		Health += RegenerationRate * DeltaTime;
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
	if (CurrentWeapon)
		CurrentWeapon->StartReload();
}

void AGamePlayCharacter::SpawnWeaponsAndAssignToSlots()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		CurrentWeapon = World->SpawnActorDeferred<AWeapon>(WeaponSlot1, GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (WeaponSlot1)
		{
			//WeaponSlot1->IndexInBackpack = index;
			CurrentWeapon->SetOwningPawn(this);
			UGameplayStatics::FinishSpawningActor(CurrentWeapon, GetTransform());
			CurrentWeapon->AttachRootComponentTo(FPPMesh, CurrentWeapon->AttachSocketNameFPP, EAttachLocation::SnapToTargetIncludingScale);
		}
	}
	// Set Current Weapon
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

UCameraComponent* AGamePlayCharacter::GetCamera()
{
	return FollowCamera;
}

void AGamePlayCharacter::PlayHit(bool bKilled, float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	LastTimeTakeDamage = GetWorld()->TimeSeconds;

	Super::PlayHit(bKilled, DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	AGamePlayPlayerController* MyPC = Cast<AGamePlayPlayerController>(Controller);
	AGamePlayHUD* MyHUD = MyPC ? Cast<AGamePlayHUD>(MyPC->GetHUD()) : NULL;
	if (MyHUD)
	{
		MyHUD->NotifyGotHit(DamageTaken, DamageEvent, PawnInstigator);
	}
}

void AGamePlayCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	Super::OnDeath(KillingDamage, DamageEvent, PawnInstigator, DamageCauser);
	if (CurrentWeapon)
	{
		CurrentWeapon->GetRootComponent()->DetachFromParent();
		USkeletalMeshComponent* WeaponMesh = CurrentWeapon->WeaponMesh;
		if (WeaponMesh)
		{
			WeaponMesh->SetSimulatePhysics(true);
			WeaponMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
			WeaponMesh->AddAngularImpulse(FVector(1000, 500, 0));
		}
	}
}