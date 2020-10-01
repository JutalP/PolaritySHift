#pragma once

#include "GameFramework/Actor.h"
#include "SMFieldVFXActor.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class ASMFieldVFXActor : public AActor {
	GENERATED_BODY()

public:
	ASMFieldVFXActor(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VFX)
	UStaticMeshComponent* SphereMesh = nullptr;
	UFUNCTION(BlueprintCallable)
	void SetRadius(float Radius);
};