#include "SMRealignmentComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

USMRealignmentComponent::USMRealignmentComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void USMRealignmentComponent::BeginPlay() {
	Super::BeginPlay();

	DesiredAxisPosition = GetOwner()->GetActorLocation().X;
}

void USMRealignmentComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->GetActorLocation().X != DesiredAxisPosition) {
		Realign();
	}
}

void USMRealignmentComponent::Realign() {

//  	AlignmentAxis.X = DesiredAxisPosition;
//  
// 	float AxisOffset = 0 - GetOwner()->GetActorLocation().X;
//  
// 	UE_LOG(LogTemp, Warning, TEXT("[Realignment] AxisOffset: %f"), AxisOffset);
// 
// 	FVector AlignmentVector = FVector(AxisOffset, 0.0f, 0.0f);
// 
// 	FVector DrawVectorEnd = GetOwner()->GetActorLocation() + AlignmentVector * RealignmentSpeed;
// 
// 	DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), DrawVectorEnd, FColor::Red, false, 1.0f, 0, 5.0f);
// 
//  	GetOwner()->AddActorWorldOffset(AlignmentVector * RealignmentSpeed * GetWorld()->GetDeltaSeconds());
}

void USMRealignmentComponent::SetNewLocation(float Horizontal, float Vertical) {
	AlignmentAxis = FVector(DesiredAxisPosition, Horizontal, Vertical);
}
