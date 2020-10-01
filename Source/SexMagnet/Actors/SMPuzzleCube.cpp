#include "SMPuzzleCube.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SMMagnetComponent.h"
#include "Components/SMMovableComponent.h"
#include "SexMagnetCharacter.h"
#include "Systems/SMMagnetSystem.h"
#include "VFX/SMBeamVFXSystem.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

ASMPuzzleCube::ASMPuzzleCube(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	BoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);

	CubeMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(RootComponent);

	MagnetComponent = ObjectInitializer.CreateDefaultSubobject<USMMagnetComponent>(this, TEXT("MagnetComponent"));
	MovableComponent = ObjectInitializer.CreateDefaultSubobject<USMMovableComponent>(this, TEXT("MovableComponent"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ASMPuzzleCube::BeginPlay() {
	Super::BeginPlay();

	MagnetComponent->SMMovable = MovableComponent;

	MagnetComponent->OnCharacterStayPositiveBeam.AddDynamic(this, &ASMPuzzleCube::AttachMovable);
	MagnetComponent->OnCharacterStayNegativeBeam.AddDynamic(this, &ASMPuzzleCube::AttachMovable);
}

void ASMPuzzleCube::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (!MovableComponent->AttachedToActor)
		CheckForGround();
}

void ASMPuzzleCube::AttachMovable(ASexMagnetCharacter* Character, const FVector& Direction) {
	if(bSnapped){
		if (MovableComponent->AttachedToActor)
			MovableComponent->Detach();
		return;
	}

	if (!Character->MagnetSystem->bAllowedToUsebox)
		return;

	AActor* ActorToAttachTo = (AActor*)Character->MagnetSystem->BeamVFXSystem->CurrentField;

	if (bPositive && Character->MagnetComponent->bIsPositive) {
		
		if (MovableComponent->bCarriedNegative) {
			MovableComponent->Detach();
			return;
		}
		MovableComponent->AttachToActor(ActorToAttachTo, Character, Character->MagnetSystem->bPositiveCharge);
	}
	
	if (bNegative && Character->MagnetComponent->bIsNegative) {
		if (MovableComponent->bCarriedPositive) {
			MovableComponent->Detach();
			return;
		}
		MovableComponent->AttachToActor(ActorToAttachTo, Character, Character->MagnetSystem->bPositiveCharge);
	}
}

void ASMPuzzleCube::CheckForGround() {
 	FVector StartLocation = GetActorLocation();
	StartLocation.Z -= FallOffset;
 	FVector EndLocation = StartLocation + -GetActorUpVector() * GroundTraceLength;
 
 	FCollisionQueryParams Parameters;
 	Parameters.AddIgnoredActor(this);
 
 	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, -1.0f, 0, 5.0f);
 
 	FHitResult Hit;
 	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Parameters);
 
 	if (!bHit) {
 		FallDown();
 	}
}

void ASMPuzzleCube::FallDown() {

	FVector FallVector = FVector(0.0f, 0.0f, -FallSpeed * GetWorld()->GetDeltaSeconds());
	AddActorWorldOffset(FallVector);
}
