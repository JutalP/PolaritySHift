#pragma once

#include "Animation/AnimInstance.h"
#include "SMGolemAnimInstance.generated.h"

class ASexMagnetCharacter;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnBeginIdleSignature, FRotator, BoneRotation);

UCLASS()
class USMGolemAnimInstance : public UAnimInstance {

	GENERATED_BODY()

public:
	USMGolemAnimInstance(const FObjectInitializer& ObjectInitializer);

	void NativeBeginPlay() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;

	void UpdateMovementVariables();

	void SetIsMoving();

	UFUNCTION(BlueprintCallable)
	void SetIdleBoneRotation();

	UPROPERTY(BlueprintAssignable)
	FSMOnBeginIdleSignature OnBeginIdle;

	UPROPERTY(BlueprintReadOnly)
	ASexMagnetCharacter* OwnerCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly)
	USkeletalMeshComponent* OwnerMesh = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FVector MovementDirection;

	UPROPERTY(BlueprintReadOnly)
	float DirectionMagnitude;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMoving;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BodyBoneIndex;

	UPROPERTY(BlueprintReadOnly)
	FRotator CachedBoneRotation;

	bool bIdleRotationSet = false;
};
