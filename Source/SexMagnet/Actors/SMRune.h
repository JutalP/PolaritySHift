#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMRune.generated.h"

class UStaticMeshComponent;
class USMMovableComponent;
class USMMagnetComponent;
class USphereComponent;
class UBoxComponent;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class ASMRune : public AActor {
	GENERATED_BODY()
public:
	ASMRune(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

// 	UFUNCTION()
// 	void StickToField(class ASexMagnetCharacter* Character, const FVector& Direction);
// 
// 	UFUNCTION()
// 	void LaunchFromField(class ASexMagnetCharacter* Character, const FVector& Direction);
// 
// 	UFUNCTION()
// 	void BeamAttractField(class ASexMagnetCharacter* Character, const FVector& Direction);

// 	UPROPERTY(VisibleAnywhere)
// 		UBoxComponent* BoxCollision = nullptr;
// 
// 	UPROPERTY(VisibleAnywhere)
// 		UStaticMeshComponent* CubeMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision = nullptr;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FieldMesh = nullptr;

 	UPROPERTY(VisibleAnywhere)
 	UDecalComponent* RuneDecal = nullptr;
  
  	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  	USMMagnetComponent* MagnetComponent = nullptr;
  
  	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  	USMMovableComponent* MovableComponent = nullptr;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPositive = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bNegative = true;

	UFUNCTION()
	void AttachMovable(ASexMagnetCharacter* Character, const FVector& Direction);

	UFUNCTION()
	void AttachCharacter(ASexMagnetCharacter* Character, const FVector& Direction);

	UFUNCTION()
	void LaunchCharacter(ASexMagnetCharacter* Character, const FVector& Direction);

	UPROPERTY(EditDefaultsOnly, Category = "Launch properties")
	float LaunchSpeed = 900.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Launch properties")
	float DefaultHorizontalLaunch = 900.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Launch properties")
	float DefaultVerticalLaunch = 900.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Attach to field properties")
	float AttachSpeed = 3.0f;

// 	UPROPERTY(EditAnywhere, Category = Launch)
// 	float LaunchSpeedMultiplier = 500.f;
// 	UPROPERTY(EditAnywhere, Category = Beam)
// 	float FieldMoveSpeedMultiplier = 1.f;
// 	UPROPERTY(EditAnywhere, Category = Magnetism)
// 	bool bPositive = false;
};