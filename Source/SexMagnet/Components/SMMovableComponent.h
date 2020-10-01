#pragma once

#include "Components/ActorComponent.h"
#include "SMMovableComponent.generated.h"

class ASexMagnetCharacter;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class USMMovableComponent : public UActorComponent {
	GENERATED_BODY()
public:
	USMMovableComponent(const FObjectInitializer& ObjectInitializer);
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AttachToActor(AActor* Actor, ASexMagnetCharacter* Attacher, bool bAddedPositive);

	UFUNCTION()
	void Detach();
	
	UFUNCTION(BlueprintCallable)
	void SetSexCharacter(ASexMagnetCharacter* CharToSet, bool bAddedPositive);

	UPROPERTY(EditDefaultsOnly)
	float FieldMoveSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float ReleaseThreshold = 400.0f;

	UPROPERTY(BlueprintReadOnly)
	bool bCarriedPositive = false;

	UPROPERTY(BlueprintReadOnly)
	bool bCarriedNegative = false;

	UFUNCTION()
	bool HasCarryPolarity();

	UFUNCTION()
	void ResetCarryPolarity();

	void SetCarriedPositive();
	void SetCarriedNegative();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASexMagnetCharacter* AttacherCharacter = nullptr;

	UPROPERTY()
	AActor* AttachedToActor = nullptr;
	UPROPERTY()
	ASexMagnetCharacter* CarriedCharacter = nullptr;
};