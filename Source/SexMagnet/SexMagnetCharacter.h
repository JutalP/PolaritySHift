#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SexMagnetCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnRequestResetSignature, ASexMagnetCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnLandedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnDisengageBeamSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnMoveCameraSignature, const FVector&, Direction, float, DeltaSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnZoomCameraSignature, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnIncreaseCameraSpeedSignature, float, Value);

class USMMagnetSystem;
class USMFieldVFXComponent;
class USMMagnetComponent;
class USMMovableComponent;
class USMCharacterVFXComponent;
class APlayerController;
class USMRealignmentComponent;
class ASMPuzzleCube;

UCLASS(config = Game)
class ASexMagnetCharacter : public ACharacter {
	GENERATED_BODY()

	ASexMagnetCharacter();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magnetism)
	USMMagnetComponent* MagnetComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magnetism)
	USMMagnetSystem* MagnetSystem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magnetism)
	USMMovableComponent* MovableComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Events)
	USMCharacterVFXComponent* VFXComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Alignment)
	USMRealignmentComponent* RealignmentComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FVector MoveDirection;

	UPROPERTY(EditAnywhere, Category = "Fall Damage")
	float FallDamageThreshold = 3000.0f;

	UPROPERTY(EditAnywhere, Category = Consent)
	bool bRequirePickupConsent = true;
	UPROPERTY()
	bool bIsCarried = false;

	UPROPERTY()
	bool bIsDead = false;

	UPROPERTY()
	bool bIsPaused = false;

	bool bLaunchedOffCharacter = false;

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void Revive();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void MoveRight(float Value);
	void MoveUp(float Value);

	void VFXEngagePositiveField();
	void VFXEngageNegativeField();

	void VFXOnJumpedEvent();
	void VFXOnLandedEvent();

	void VFXOnDiedEvent();

	void EngagePositiveField(float Value);
	void EngageNegativeField(float Value);
	void PointVertical(float Value);
	void PointHorizontal(float Value);

	void PauseGame();
	void RestartLevel();

	void Jump() override;
	void Landed(const FHitResult& Hit) override;
	void DoRagdoll();
	void PushWithBeam(ASexMagnetCharacter* OtherActor, const FVector& Direction);

	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController();

	UFUNCTION()
	void BroadcastDisengageBeam();

	UFUNCTION()
	void NotifyInPositiveField(ASexMagnetCharacter* OtherActor, const FVector& Direction);

	UFUNCTION()
	void NotifyInNegativeField(ASexMagnetCharacter* OtherActor, const FVector& Direction);

	UFUNCTION()
	void NotifyInPositiveBeam(ASexMagnetCharacter* OtherActor, const FVector& Direction);

	UFUNCTION()
	void NotifyInNegativeBeam(ASexMagnetCharacter* OtherActor, const FVector& Direction);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayForceFieldSound();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayBeamSound();

	UFUNCTION(BlueprintImplementableEvent)
	void PauseWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveWidget();

	UFUNCTION()
	void MoveCameraRight(float Value);

	UFUNCTION()
	void MoveCameraUp(float Value);

	UFUNCTION()
	void ZoomCamera(float Value);

	UFUNCTION()
	void IncreaseCameraSpeed(float Value);

	bool bIsPlaying = false;

	UPROPERTY()
	ASMPuzzleCube* CarriedCube = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FSMOnRequestResetSignature OnRequestReset;

	UPROPERTY(BlueprintAssignable)
	FSMOnLandedSignature OnCharacterLanded;

	UPROPERTY(BlueprintAssignable)
	FSMOnDisengageBeamSignature OnDisengageBeam;

	UPROPERTY(BlueprintAssignable)
	FSMOnMoveCameraSignature OnMoveCamera;

	UPROPERTY(BlueprintAssignable)
	FSMOnZoomCameraSignature OnZoomCamera;

	UPROPERTY(BlueprintAssignable)
	FSMOnIncreaseCameraSpeedSignature OnIncreaseCameraSpeed;
};