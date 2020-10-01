#pragma once

#include "GameFramework/Actor.h"
#include "SMPuzzleCube.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USMMagnetComponent;
class USMMovableComponent;
class ASexMagnetCharacter;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class ASMPuzzleCube : public AActor {

	GENERATED_BODY()

public:
	ASMPuzzleCube(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USMMagnetComponent* MagnetComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USMMovableComponent* MovableComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPositive = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bNegative = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSnapped = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FallSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FallOffset = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GroundTraceLength = 20.0f;

	void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void AttachMovable(ASexMagnetCharacter* Character, const FVector& Direction);

	void CheckForGround();
	void FallDown();
};
