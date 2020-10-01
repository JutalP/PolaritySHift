#pragma once

#include "GameFramework/Actor.h"
#include "SMCheckPoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnCheckPointReachedSignature, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnRespawnSignature);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class ASMCheckPoint : public AActor {

	GENERATED_BODY()

public:
	ASMCheckPoint(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	bool bReached = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Index;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsActivated = false;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;

	void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnEmitter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRespawn();

	UPROPERTY(BlueprintCallable)
	FSMOnCheckPointReachedSignature OnCheckpointReached;

	UPROPERTY(BlueprintCallable)
	FSMOnRespawnSignature OnRespawnDelegate;
};