#pragma once

#include "GameFramework/Actor.h"
#include "SMCoopCamera.generated.h"

UCLASS()
class ASMCoopCamera : public AActor {
	GENERATED_BODY()
public:
	ASMCoopCamera(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Debug)
	bool bDrawDebug = false;
	UPROPERTY(EditDefaultsOnly, Category = "Vertical Movement")
	bool bAdjustHeightOnLanding = true;
	UPROPERTY(EditDefaultsOnly, Category = "Vertical Movement")
	float CameraZOffset = 150.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Vertical Movement")
	float VerticalMoveSpeed = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Vertical Movement")
	float IncreaseSpeedZThreshold = 40.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Vertical Movement")
	float VerticalMoveSpeedIncreaseMultiplier = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Horizontal Movement")
	float HorizontalMoveSpeed = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Vertical Movement")
	float MaximumVerticalOffset = 150.0f;

	void Initialize(class ASexMagnetCharacter* PlayerOne, class ASexMagnetCharacter* PlayerTwo = nullptr);
	void Update(float DeltaSeconds, const FVector& FirstPos, const FVector& SecondPos);
	UFUNCTION()
	void ToggleUpdateZOffset();
private:
	float ZLerpAlpha = 0.0f;
	float CurrentLowestZ = 0.0f;

	void UpdateVerticalPosition(float DeltaSeconds, FVector& OutLocation, float NewHeight);
	void UpdateHorizontalPosition(float DeltaSeconds, FVector& OutLocation, float NewPositionY);

	void AccomodateFastFallers();
	float TmpVerticalMoveSpeed;
	float IncreasedCameraSpeed;
};