#include "SMMagnetComponent.h"
#include "SexMagnetCharacter.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SMMovableComponent.h"
#include "Systems/SMMagnetSystem.h"
#include "VFX/SMBeamVFXSystem.h"

void USMMagnetComponent::EnableMagnetism() {
	UE_LOG(LogTemp, Warning, TEXT("[MagnetComponent] Magnetism is back on the menu"));
	bIsDisrupted = false;
}

void USMMagnetComponent::SetMovableComponent(USMMovableComponent* Movable){
	SMMovable = Movable;
}

void USMMagnetComponent::NotifyNegativeBeam_Implementation(ASexMagnetCharacter* Instigator, const FVector& Direction, float Value) {
	OnCharacterStayPositiveBeam.Broadcast(Instigator, Direction);
}

void USMMagnetComponent::NotitfyPositiveBeam_Implementation(ASexMagnetCharacter* Instigator, const FVector& Direction, float Value) {
	OnCharacterStayNegativeBeam.Broadcast(Instigator, Direction);
}

void USMMagnetComponent::SetIsPositive_Implementation(bool Value) {
	bIsPositive = Value;
}

void USMMagnetComponent::SetIsNegative_Implementation(bool Value) {
	bIsNegative = Value;
}

void USMMagnetComponent::NotifyPositiveField_Implementation(ASexMagnetCharacter* Instigator, const FVector& Direction, float Value) {
	if (bIsDisrupted == false) {
		OnStayPositiveField.Broadcast(Instigator, Value);
		OnCharacterStayPositiveField.Broadcast(Instigator, Direction);
	}
}

void USMMagnetComponent::NotifyNegativeField_Implementation(ASexMagnetCharacter* Instigator, const FVector& Direction, float Value) {
	if (bIsDisrupted == false) {
		OnStayNegativeField.Broadcast(Instigator, Value);
		OnCharacterStayNegativeField.Broadcast(Instigator, Direction);
	}
}