#pragma once

#include "Components/ActorComponent.h"
#include "SMCharacterVFXComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnEngagePositiveFieldSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnEngageNegativeFieldSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnCharacterJumpedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnCharacterLandedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnCharacterDiedSignature);


UCLASS()
class USMCharacterVFXComponent : public UActorComponent {

	GENERATED_BODY()

public:
	USMCharacterVFXComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void BroadcastEngagePositiveField();

	UFUNCTION()
	void BroadcastEngageNegativeField();

	UPROPERTY(BlueprintAssignable)
	FSMOnEngagePositiveFieldSignature OnEngagePositiveField;

	UPROPERTY(BlueprintAssignable)
	FSMOnEngageNegativeFieldSignature OnEngageNegativeField;

	UPROPERTY(BlueprintAssignable)
	FSMOnCharacterJumpedSignature OnCharacterJumped;

	UPROPERTY(BlueprintAssignable)
	FSMOnCharacterLandedSignature OnCharacterLanded;

	UPROPERTY(BlueprintAssignable)
	FSMOnCharacterDiedSignature OnCharacterDied;
};
