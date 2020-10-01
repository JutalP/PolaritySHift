#include "SexMagnetCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SMMagnetComponent.h"
#include "Systems/SMMagnetSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SMMovableComponent.h"
#include "VFX/SMBeamVFXSystem.h"
#include "Components/SMCharacterVFXComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SMRealignmentComponent.h"
#include "Actors/SMPuzzleCube.h"

ASexMagnetCharacter::ASexMagnetCharacter() {
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetConstraintMode(EDOFMode::YZPlane);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MagnetComponent = CreateDefaultSubobject<USMMagnetComponent>(TEXT("MagnetComponent"));
	MagnetSystem = CreateDefaultSubobject<USMMagnetSystem>(TEXT("MagnetSystem"));
	MovableComponent = CreateDefaultSubobject<USMMovableComponent>(TEXT("MovableComponent"));
	VFXComponent = CreateDefaultSubobject<USMCharacterVFXComponent>(TEXT("VFXComponent"));
	RealignmentComponent = CreateDefaultSubobject<USMRealignmentComponent>(TEXT("RealignmentComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
}

void ASexMagnetCharacter::MoveCameraRight(float Value) {
	FVector Direction = FVector(0.0f, -1.0f * Value, 0.0f);
	OnMoveCamera.Broadcast(Direction, GetWorld()->GetDeltaSeconds());
}

void ASexMagnetCharacter::MoveCameraUp(float Value) {
	FVector Direction = FVector(0.0f, 0.0f, 1.0f * Value);
	OnMoveCamera.Broadcast(Direction, GetWorld()->GetDeltaSeconds());
}

void ASexMagnetCharacter::ZoomCamera(float Value) {
	OnZoomCamera.Broadcast(Value);
}

void ASexMagnetCharacter::IncreaseCameraSpeed(float Value) {
	OnIncreaseCameraSpeed.Broadcast(Value);
}

void ASexMagnetCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &ASexMagnetCharacter::RestartLevel).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ASexMagnetCharacter::PauseGame).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAxis("MoveRight", this, &ASexMagnetCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASexMagnetCharacter::MoveUp);
	PlayerInputComponent->BindAxis("RightTriggerAxis", this, &ASexMagnetCharacter::EngagePositiveField);
	PlayerInputComponent->BindAction("RightTriggerButton", IE_Pressed, this, &ASexMagnetCharacter::VFXEngagePositiveField);
	PlayerInputComponent->BindAction("RightTriggerButton", IE_Released, this, &ASexMagnetCharacter::BroadcastDisengageBeam);
	PlayerInputComponent->BindAxis("LeftTriggerAxis", this, &ASexMagnetCharacter::EngageNegativeField);
	PlayerInputComponent->BindAction("LeftTriggerButton", IE_Pressed, this, &ASexMagnetCharacter::VFXEngageNegativeField);
	PlayerInputComponent->BindAction("LeftTriggerButton", IE_Released, this, &ASexMagnetCharacter::BroadcastDisengageBeam);
	PlayerInputComponent->BindAxis("LookUp", this, &ASexMagnetCharacter::PointVertical);
	PlayerInputComponent->BindAxis("LookRight", this, &ASexMagnetCharacter::PointHorizontal);

	PlayerInputComponent->BindAxis("CamMoveUp", this, &ASexMagnetCharacter::MoveCameraUp);
	PlayerInputComponent->BindAxis("CamMoveRight", this, &ASexMagnetCharacter::MoveCameraRight);
	PlayerInputComponent->BindAxis("CamZoom", this, &ASexMagnetCharacter::ZoomCamera);
	PlayerInputComponent->BindAxis("CamIncreaseSpeed", this, &ASexMagnetCharacter::IncreaseCameraSpeed);
}

void ASexMagnetCharacter::BeginPlay() {
	Super::BeginPlay();
	MagnetSystem->Initialize(this);
	MagnetComponent->SetMovableComponent(MovableComponent);
	MagnetComponent->OnCharacterStayPositiveField.AddDynamic(this, &ASexMagnetCharacter::NotifyInPositiveField);
	MagnetComponent->OnCharacterStayNegativeField.AddDynamic(this, &ASexMagnetCharacter::NotifyInNegativeField);
	MagnetComponent->OnCharacterStayPositiveBeam.AddDynamic(this, &ASexMagnetCharacter::NotifyInPositiveBeam);
	MagnetComponent->OnCharacterStayNegativeBeam.AddDynamic(this, &ASexMagnetCharacter::NotifyInNegativeBeam);
}

void ASexMagnetCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (MagnetSystem && !bIsDead) {
		MagnetSystem->Update();
	}
}

void ASexMagnetCharacter::MoveRight(float Value) {
	MoveDirection.X = -Value;
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER && !bIsDead) {
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
	}
}

void ASexMagnetCharacter::MoveUp(float Value) {
	MoveDirection.Z = Value;
}

void ASexMagnetCharacter::VFXEngagePositiveField() {
	VFXComponent->OnEngagePositiveField.Broadcast();
}

void ASexMagnetCharacter::VFXEngageNegativeField() {
	VFXComponent->OnEngageNegativeField.Broadcast();
}

void ASexMagnetCharacter::VFXOnJumpedEvent() {
	VFXComponent->OnCharacterJumped.Broadcast();
}

void ASexMagnetCharacter::VFXOnLandedEvent() {
	VFXComponent->OnCharacterLanded.Broadcast();
}

void ASexMagnetCharacter::VFXOnDiedEvent() {
	VFXComponent->OnCharacterDied.Broadcast();
}

void ASexMagnetCharacter::EngagePositiveField(float Value) {
	if (MagnetSystem && !bIsDead) {
		MagnetSystem->EngagePositiveField(Value);
		if (Value > KINDA_SMALL_NUMBER && !bIsPlaying) {
			bIsPlaying = true;
			PlayForceFieldSound();
		}
	}
}

void ASexMagnetCharacter::EngageNegativeField(float Value) {
	if (MagnetSystem && !bIsDead) {
		MagnetSystem->EngageNegativeField(Value);
		if (Value > KINDA_SMALL_NUMBER && !bIsPlaying) {
			bIsPlaying = true;
			PlayForceFieldSound();
		}
	}
}

void ASexMagnetCharacter::PointVertical(float Value) {
	if (MagnetSystem && !bIsDead) {
		MagnetSystem->PointVertical(Value);
	}
}

void ASexMagnetCharacter::PointHorizontal(float Value) {
	if (MagnetSystem && !bIsDead) {
		MagnetSystem->PointHorizontal(Value);
	}
}

void ASexMagnetCharacter::PauseGame() {
	if (!bIsPaused) {
		bIsPaused = true;
		UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);
		PauseWidget();
	} else {
		bIsPaused = false;
		UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);
		RemoveWidget();
	}
}

void ASexMagnetCharacter::RestartLevel() {
	// spawn both players at last checkpoint (kill both players) reset puzzle

	OnRequestReset.Broadcast(nullptr);

	//UGameplayStatics::OpenLevel(GetWorld(), "?restart");
}

void ASexMagnetCharacter::Jump() {
	Super::Jump();

	MagnetSystem->bAllowedToUsebox = true;
	VFXOnJumpedEvent();
}

void ASexMagnetCharacter::Landed(const FHitResult& Hit) {
	// 	if (FMath::Abs(GetMovementComponent()->Velocity.Z) >= FallDamageThreshold) {
	// 		Die();
	// 	}

	bLaunchedOffCharacter = false;

	if (Hit.GetActor() && Hit.GetActor()->IsA<ASMPuzzleCube>()) {
		MagnetSystem->bAllowedToUsebox = false;
		if (CarriedCube)
			CarriedCube->MovableComponent->Detach();
	} else
		MagnetSystem->bAllowedToUsebox = true;

	VFXOnLandedEvent();
	OnCharacterLanded.Broadcast();
	Super::Landed(Hit);
}

void ASexMagnetCharacter::Die() {
	if (bIsDead) {
		return;
	}

	VFXOnDiedEvent();
	bIsDead = true;
	DoRagdoll();
	GetMovementComponent()->SetComponentTickEnabled(false);
}

void ASexMagnetCharacter::Revive() {
	bIsDead = false;
	UE_LOG(LogTemp, Warning, TEXT("[MagnetCharacter] Ressurected!"));

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(FName(TEXT("CharacterMesh")));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMovementComponent()->SetComponentTickEnabled(true);
	GetMovementComponent()->SetActive(true);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetMesh()->SetWorldLocationAndRotation(GetCapsuleComponent()->GetComponentLocation(), FQuat::Identity);
}

void ASexMagnetCharacter::DoRagdoll() {
	if (!bIsDead) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[MagnetCharacter] %s died"), *GetName());
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName(TEXT("Ragdoll")));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMovementComponent()->SetActive(false);
}

APlayerController* ASexMagnetCharacter::GetPlayerController() {
	return Cast<APlayerController>(GetController());
}

void ASexMagnetCharacter::BroadcastDisengageBeam() {
	OnDisengageBeam.Broadcast();
}

void ASexMagnetCharacter::NotifyInPositiveField(ASexMagnetCharacter* OtherActor, const FVector& Direction) {
	//if(MagnetComponent->bIsNegative && bIsDead){
	//	Revive();
	//	return;
	//}

	if (bLaunchedOffCharacter)
		return;

	//Maybe add hit point to launch in the opposite direction of collision instead of up?
	if (MagnetComponent->bIsPositive && !OtherActor->GetMovementComponent()->IsMovingOnGround()) {
		OtherActor->LaunchCharacter(FVector::UpVector * 900.0f, false, true);
		bLaunchedOffCharacter = true;
	}
}

void ASexMagnetCharacter::NotifyInNegativeField(ASexMagnetCharacter* OtherActor, const FVector& Direction) {
	//if(MagnetComponent->bIsPositive && bIsDead){
	//	Revive();
	//	return;
	//}

	if (bLaunchedOffCharacter)
		return;

	if (MagnetComponent->bIsNegative && !OtherActor->GetMovementComponent()->IsMovingOnGround()) {
		OtherActor->LaunchCharacter(FVector::UpVector * 900.0f, false, true);
		bLaunchedOffCharacter = true;
	}
}

void ASexMagnetCharacter::NotifyInPositiveBeam(ASexMagnetCharacter* OtherActor, const FVector& Direction) {

	if (bRequirePickupConsent)
		if (!MagnetComponent->bIsNegative)
			return;

// 	if (MovableComponent->bCarriedNegative) {
// 		MovableComponent->ResetCarryPolarity();
// 		return;
// 	}

	MovableComponent->AttachToActor((AActor*)OtherActor->MagnetSystem->BeamVFXSystem->CurrentField, OtherActor, OtherActor->MagnetSystem->bPositiveCharge);
	bIsCarried = true;
	GetMovementComponent()->Velocity = FVector::ZeroVector;
	GetMovementComponent()->SetComponentTickEnabled(false);
	MovableComponent->SetSexCharacter(this, false);
}

void ASexMagnetCharacter::NotifyInNegativeBeam(ASexMagnetCharacter* OtherActor, const FVector& Direction) {

	if (bRequirePickupConsent)
		if (!MagnetComponent->bIsPositive)
			return;
// 
// 	if (MovableComponent->HasCarryPolarity()) {
// 		MovableComponent->ResetCarryPolarity();
// 		return;
// 	}
// 
	MovableComponent->AttachToActor((AActor*)OtherActor->MagnetSystem->BeamVFXSystem->CurrentField, OtherActor, OtherActor->MagnetSystem->bPositiveCharge);
	bIsCarried = true;
	GetMovementComponent()->Velocity = FVector::ZeroVector;
	GetMovementComponent()->SetComponentTickEnabled(false);
	MovableComponent->SetSexCharacter(this, true);
}
//
//void ASexMagnetCharacter::PositiveGolemInteraction(ASexMagnetCharacter* OtherActor){
//
//}
//
//void ASexMagnetCharacter::NegativeGolemInteraction(ASexMagnetCharacter* OtherActor){
//
//}