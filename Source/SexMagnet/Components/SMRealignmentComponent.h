#pragma once

#include "Components/ActorComponent.h"
#include "SMRealignmentComponent.generated.h"

UCLASS()
class USMRealignmentComponent : public UActorComponent {

	GENERATED_BODY()

public:
	USMRealignmentComponent(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void Realign();

	void SetNewLocation(float Horizontal, float Vertical);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Alignment)
	FVector AlignmentAxis;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Alignment)
	float RealignmentSpeed = 150.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Alignment)
	float DesiredAxisPosition;
};
