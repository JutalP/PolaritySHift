#pragma once

#include "Components/PrimitiveComponent.h"
#include "SMMagnetSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnPositiveField, TArray<USMMagnetComponent*>, MagnetsInRange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnNegativeField, TArray<USMMagnetComponent*>, MagnetsInRange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnDisableDisruptionField);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnSpecifyLookDirection, const FVector&, LookDirection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnStopSpecifyLookDirection);

class USphereComponent;
class USMMagnetComponent;
class ASexMagnetCharacter;
class ASMFieldVFXActor;
class AActor;
class UParticleSystem;
class UParticleSystemComponent;
class USMBeamVFXSystem;

UCLASS(meta = (BlueprintSpawnableComponent))
class USMMagnetSystem : public UPrimitiveComponent {
	GENERATED_BODY()

public:
	USMMagnetSystem(const FObjectInitializer& ObjectInitializer);
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Initialize(AActor* InActor);

	UFUNCTION()
	void EngagePositiveField(float Value);

	UFUNCTION()
	void EngageNegativeField(float Value);

	void EngageField(float Value);
	void UnEngageField();
	void SetFieldRadius(float Value);
	void SetOwnerMagnetPol();
	void EnableField();
	void DisableFields();
	void ClearParameters();

	UFUNCTION()
	void DirectField();

	UFUNCTION()
	void StopDirectField();
	void EngageBeam(float Value);

	UFUNCTION(BlueprintCallable)
	void EngageDisruptionField();

	UFUNCTION(BlueprintCallable)
	void DisableDisruptionField();

	UFUNCTION()
	void PointVertical(float Value);

	UFUNCTION()
	void PointHorizontal(float Value);
	void Update();

	// Collision --------------------------------
	void GetMagnetsInRadius(TArray<USMMagnetComponent*>& OutMagnets, float Radius);
	void GetMagnetsAlongLine(TArray<USMMagnetComponent*>& OutMagnets, const FVector& Direction, float Range, bool bDrawDebugLines = false);

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<USMBeamVFXSystem> BeamVFXSystemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USMBeamVFXSystem* BeamVFXSystem = nullptr;
 
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<ASMFieldVFXActor> PositiveFieldClass;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSubclassOf<ASMFieldVFXActor> NegativeFieldClass;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	ASMFieldVFXActor* PositiveField = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	ASMFieldVFXActor* NegativeField = nullptr;

	UPROPERTY(EditAnywhere, Category = "Golem jump trace")
	float TraceDistance = 100.0f;

	UPROPERTY(VisibleAnywhere)
	bool bPositiveCharge = false;

	UPROPERTY(VisibleAnywhere)
	bool bNegativeCharge = false;
	bool bPositivePrevFrame = false;

	UPROPERTY(EditAnywhere, Category = "Field properties")
	float MinimumFieldRadius = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Field properties")
	float FieldRadius = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Field properties")
	float FieldGrowthRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam properties")
	float TriggerHeldThreshold = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam properties")
	float BeamMaxDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam properties")
	float BeamMinDistance = 0.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam properties")
	float BeamSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam properties")
	float BeamGrowthRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam properties")
	float TmpBeamDistance = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bAllowedToUsebox = true;

	UPROPERTY(BlueprintReadOnly)
	FVector LookDirection;
	float LookDirectionZ = 0;
	float LookDirectionY = 0;

	UPROPERTY()
	AActor* OwnerActor = nullptr;

	UPROPERTY()
	ASexMagnetCharacter* OwnerCharacter = nullptr;

	UPROPERTY()
	USMMagnetComponent* OwnerMagnet = nullptr;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnPositiveField OnEngagePositiveField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnNegativeField OnEngageNegativeField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnDisableDisruptionField OnDisableDisruption; 

	FCollisionShape CachedCollisionShape;

	UPROPERTY(EditAnywhere, Category = Magnetism)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, Category = Magnetism)
	TArray<TSubclassOf<AActor>> ActorsToIgnore;

	FCollisionObjectQueryParams CachedCollisionObjectParams;

	FCollisionQueryParams CachedCollisionQueryParams;

	UPROPERTY(BlueprintAssignable)
	FSMOnSpecifyLookDirection OnSpecifyLookDirection;

	UPROPERTY(BlueprintAssignable)
	FSMOnStopSpecifyLookDirection OnStopSpecifyLookDirection;
private:
	float TmpRadius = 0.0f;
	float RadiusLerpAlpha = 0.0f;
};