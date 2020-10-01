#include "SMGolemAnimInstance.h"
#include "SexMagnetCharacter.h"

USMGolemAnimInstance::USMGolemAnimInstance(const FObjectInitializer& ObjectInitializer) {

}

void USMGolemAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ASexMagnetCharacter>(TryGetPawnOwner());
	ensure(OwnerCharacter);
	
	OwnerMesh = OwnerCharacter->GetMesh();
	ensure(OwnerMesh);

	BodyBoneIndex = OwnerMesh->GetBoneIndex(TEXT("body_jnt"));
}

void USMGolemAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateMovementVariables();

	if (!bIdleRotationSet)
		SetIdleBoneRotation();
}

void USMGolemAnimInstance::UpdateMovementVariables() {
	
	if (OwnerCharacter) {
		MovementDirection = OwnerCharacter->MoveDirection;
		DirectionMagnitude = MovementDirection.Size();
	}
}

void USMGolemAnimInstance::SetIsMoving() {
	if (DirectionMagnitude > KINDA_SMALL_NUMBER) {
		bIsMoving = true;
	} else {
		FTransform BoneTransform = OwnerMesh->GetBoneTransform(BodyBoneIndex);
		FRotator BoneRotation = BoneTransform.Rotator();
		CachedBoneRotation = BoneRotation;
		OnBeginIdle.Broadcast(BoneRotation);
		bIsMoving = false;
	}
}

void USMGolemAnimInstance::SetIdleBoneRotation() {
	bIdleRotationSet = true;
}
