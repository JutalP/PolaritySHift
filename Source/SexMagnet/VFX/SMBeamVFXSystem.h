#pragma once

#include "CoreMinimal.h"
#include "SMBeamVFXSystem.generated.h"

class UParticleSystemComponent;
class UParticleSystem;
class ASMFieldVFXActor;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class USMBeamVFXSystem : public UObject {

	GENERATED_BODY()

public:
	USMBeamVFXSystem(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void InitializeBeam();

	UFUNCTION(BlueprintCallable)
	void EnableBeam(const FVector& StartLocation, bool bPositive);

	UFUNCTION(BlueprintCallable)
	void DisableBeam();

	UFUNCTION()
	void UpdateBeam(AActor* SourceActor, const FVector& Direction, float Distance);

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BeamVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<ASMFieldVFXActor> PositiveFieldClass;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<ASMFieldVFXActor> NegativeFieldClass;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	ASMFieldVFXActor* PositiveField = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	ASMFieldVFXActor* NegativeField = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	float FieldRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UParticleSystem* PositiveBeamVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UParticleSystem* NegativeBeamVFX = nullptr;

	bool bBeamSpawned = false;

	UPROPERTY(BlueprintReadOnly)
	ASMFieldVFXActor* CurrentField = nullptr;
};