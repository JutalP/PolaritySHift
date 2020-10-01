// Fill out your copyright notice in the Description page of Project Settings.

#include "SexMagnetPlayer2.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASexMagnetPlayer2::ASexMagnetPlayer2() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
}

// Called when the game starts or when spawned
void ASexMagnetPlayer2::BeginPlay() {
	Super::BeginPlay();

}

void ASexMagnetPlayer2::MoveRight(float Val) {
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Val);
}

// Called every frame
void ASexMagnetPlayer2::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASexMagnetPlayer2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("JumpP2", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("JumpP2", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveRightP2", this, &ASexMagnetPlayer2::MoveRight);
}

