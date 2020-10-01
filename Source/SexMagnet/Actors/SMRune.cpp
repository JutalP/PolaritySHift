#include "SMRune.h"
#include "SexMagnetCharacter.h"
#include "Components/SMMagnetComponent.h"
#include "Components/SMMovableComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Systems/SMMagnetSystem.h"
#include "VFX/SMBeamVFXSystem.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"

ASMRune::ASMRune(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

// 	BoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollision"));
// 	SetRootComponent(BoxCollision);
// 
// 	CubeMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CubeMesh"));
// 	CubeMesh->SetupAttachment(RootComponent);

	SphereCollision = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->InitSphereRadius(80.0f);

	FieldMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("FieldMesh"));
	FieldMesh->SetupAttachment(RootComponent);

 	RuneDecal = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("DecalComponent"));
 	RuneDecal->SetupAttachment(RootComponent);

	MagnetComponent = ObjectInitializer.CreateDefaultSubobject<USMMagnetComponent>(this, TEXT("MagnetComponent"));
	MovableComponent = ObjectInitializer.CreateDefaultSubobject<USMMovableComponent>(this, TEXT("MovableComponent"));
}

void ASMRune::BeginPlay() {
	Super::BeginPlay();

	if (bNegative) {
		MagnetComponent->OnCharacterStayPositiveBeam.AddDynamic(this, &ASMRune::AttachMovable);
		MagnetComponent->OnCharacterStayPositiveField.AddDynamic(this, &ASMRune::LaunchCharacter);
		MagnetComponent->OnCharacterStayNegativeField.AddDynamic(this, &ASMRune::AttachCharacter);
	}

	if (bPositive) {
		MagnetComponent->OnCharacterStayNegativeBeam.AddDynamic(this, &ASMRune::AttachMovable);
		MagnetComponent->OnCharacterStayNegativeField.AddDynamic(this, &ASMRune::LaunchCharacter);
		MagnetComponent->OnCharacterStayPositiveField.AddDynamic(this, &ASMRune::AttachCharacter);
	}

	MagnetComponent->SetMovableComponent(MovableComponent);
}

void ASMRune::AttachMovable(ASexMagnetCharacter* Character, const FVector& Direction) {

// 	if (MovableComponent->HasCarryPolarity()) {
// 		MovableComponent->ResetCarryPolarity();
// 		return;
// 	}

	AActor* ActorToAttachTo = (AActor*)Character->MagnetSystem->BeamVFXSystem->CurrentField;
	if (bPositive && Character->MagnetComponent->bIsPositive) {
		//UE_LOG(LogTemp, Warning, TEXT("[Rune] Positive attach at time: %f"), FApp::GetCurrentTime);
		MovableComponent->AttachToActor(ActorToAttachTo, Character, Character->MagnetSystem->bPositiveCharge);
	}
	
	if (bNegative && Character->MagnetComponent->bIsNegative) {
		//UE_LOG(LogTemp, Warning, TEXT("[Rune] Negative attach at time: %f"), FApp::GetCurrentTime);
		MovableComponent->AttachToActor(ActorToAttachTo, Character, Character->MagnetSystem->bPositiveCharge);
	}
}

void ASMRune::AttachCharacter(ASexMagnetCharacter* Character, const FVector& Direction) {
	Character->GetMovementComponent()->SetComponentTickEnabled(false);

	FVector NewLocation = GetActorLocation() - Character->GetActorLocation();
	NewLocation.X = 0;
	NewLocation = NewLocation * (GetWorld()->GetDeltaSeconds() * AttachSpeed);
	Character->AddActorWorldOffset(NewLocation);	
}

void ASMRune::LaunchCharacter(ASexMagnetCharacter* Character, const FVector& Direction) {
	
	Character->GetMovementComponent()->SetComponentTickEnabled(true);
	if (Direction.IsNearlyZero(0.001f)) {

		FVector ForwardVector = Character->GetActorForwardVector() * DefaultHorizontalLaunch;
		ForwardVector.Z = 0.0f;
		FVector LaunchVector = FVector(ForwardVector.X, ForwardVector.Y, DefaultVerticalLaunch);

		Character->LaunchCharacter(LaunchVector, true, true);
	} else {
		
		float Horizontal = Direction.X * DefaultHorizontalLaunch ;
		float Vertical = Direction.Z * DefaultVerticalLaunch;

		FVector LaunchVector = FVector(0.0f, Horizontal, Vertical);
		Character->LaunchCharacter(LaunchVector, true, true);
	}
}
