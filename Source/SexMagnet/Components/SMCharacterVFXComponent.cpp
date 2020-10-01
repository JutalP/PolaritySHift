#include "SMCharacterVFXComponent.h"

USMCharacterVFXComponent::USMCharacterVFXComponent(const FObjectInitializer& ObjectInitializer) {

}

void USMCharacterVFXComponent::BroadcastEngagePositiveField() {
	OnEngagePositiveField.Broadcast();
}

void USMCharacterVFXComponent::BroadcastEngageNegativeField() {
	OnEngageNegativeField.Broadcast();
}
