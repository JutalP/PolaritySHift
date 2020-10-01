#include "SMMovableComponent.h"
#include "GameFramework/Actor.h"
#include "App.h"
#include "DrawDebugHelpers.h"
#include "SexMagnetCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SMMagnetComponent.h"
#include "SMRealignmentComponent.h"
#include "Systems/SMMagnetSystem.h"

USMMovableComponent::USMMovableComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer){
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = true;
}

void USMMovableComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!AttachedToActor){
		return;
	}
	
// 	if (AttacherCharacter) {
// 		if (AttacherCharacter->MagnetSystem->bPositiveCharge && bCarriedNegative) {
// 			Detach();
// 			return;
// 		}
// 
// 		if (AttacherCharacter->MagnetSystem->bNegativeCharge && bCarriedPositive) {
// 			Detach();
// 			return;
// 		}
// 
// 		if (!bCarriedPositive && !bCarriedNegative && AttacherCharacter->MagnetSystem->bNegativeCharge || AttacherCharacter->MagnetSystem->bPositiveCharge) {
// 			Detach();
// 			return;
// 		}
// 	}
// 
// 	FString MovablePolarity;
// 	if (bCarriedPositive) {
// 		MovablePolarity = "Positive";
// 	} else if (bCarriedNegative) {
// 		MovablePolarity = "Negative";
// 	} else {
// 		MovablePolarity = "None";
// 	}
// 
// 	FString Polarity;
// 	if (AttacherCharacter->MagnetSystem->bPositiveCharge) {
// 		Polarity = "Positive";
// 	} else if (AttacherCharacter->MagnetSystem->bNegativeCharge) {
// 		Polarity = "Negative";
// 	} else {
// 		Polarity = "None";
// 	}
// 
// 	UE_LOG(LogTemp, Warning, TEXT("]FuckingMovableComp[ CarrieDdPolr: %s, Attachee Pol: %s "), *MovablePolarity, *Polarity);

	FVector MoveDirection = AttachedToActor->GetActorLocation() - GetOwner()->GetActorLocation();
	FVector MoveAmount = MoveDirection * FieldMoveSpeed * FApp::GetDeltaTime();

	if (MoveAmount.Size() > ReleaseThreshold) {
		Detach();
		return;
	}

	MoveAmount.X = 0;
	GetOwner()->AddActorWorldOffset(MoveAmount, true);

	if (!CarriedCharacter)
		return;

	if(CarriedCharacter->bRequirePickupConsent){
		if(bCarriedPositive){
			if(!CarriedCharacter->MagnetComponent->bIsPositive){
				Detach();
				return;
			}
		} else{
			if(!CarriedCharacter->MagnetComponent->bIsNegative){
				Detach();
				return;
			}
		}
	}
}

void USMMovableComponent::AttachToActor(AActor* Actor, ASexMagnetCharacter* Attacher, bool bAddedPositive){
	AttachedToActor = Actor;
	PrimaryComponentTick.SetTickFunctionEnable(true);

	AttacherCharacter = Attacher;
	bAddedPositive ? SetCarriedPositive() : SetCarriedNegative();

// 	FString Polarity;
// 	if (bCarriedPositive) {
// 		Polarity = "Positive";
// 	} else if (bCarriedNegative) {
// 		Polarity = "Negative";
// 	} else {
// 		Polarity = "None";
// 	}
	//UE_LOG(LogTemp, Warning, TEXT("[MovableComponent] Attaching with polarity: %s"), *Polarity);
}

void USMMovableComponent::Detach(){
	if(!AttachedToActor){
		return;
	}

	AttacherCharacter = nullptr;
	//UE_LOG(LogTemp, Warning, TEXT("[MovableComponent] Detaching at %f"), FApp::GetCurrentTime());
	ResetCarryPolarity();
	AttachedToActor = nullptr;
	PrimaryComponentTick.SetTickFunctionEnable(false);

	if(!CarriedCharacter){
		return;
	}
	CarriedCharacter->bIsCarried = false;
	CarriedCharacter->GetMovementComponent()->SetComponentTickEnabled(true);
	CarriedCharacter = nullptr;
}

void USMMovableComponent::SetSexCharacter(ASexMagnetCharacter* CharToSet, bool bAddedPositive){
	CarriedCharacter = CharToSet;
}

bool USMMovableComponent::HasCarryPolarity() {
	return bCarriedPositive || bCarriedNegative ? true : false;
}

void USMMovableComponent::ResetCarryPolarity() {

// 	FString Polarity;
// 	if (bCarriedPositive) {
// 		Polarity = "Positive";
// 	} else if (bCarriedNegative) {
// 		Polarity = "Negative";
// 	} else {
// 		Polarity = "None";
// 	}

	//UE_LOG(LogTemp, Warning, TEXT("[MovableComponent] ResetCarryPolarity, polarity was: %s"), *Polarity);
	bCarriedPositive = false;
	bCarriedNegative = false;
}

void USMMovableComponent::SetCarriedPositive() {
	bCarriedPositive = true;
	bCarriedNegative = false;
}

void USMMovableComponent::SetCarriedNegative() {
	bCarriedNegative = true;
	bCarriedPositive = false;
}
