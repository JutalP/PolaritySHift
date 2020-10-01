#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SexMagnetGameMode.generated.h"

UCLASS(minimalapi)
class ASexMagnetGameMode: public AGameModeBase{
	GENERATED_BODY()
public:
	ASexMagnetGameMode(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
	void SetCurrentCheckpoint(int32 Index);

	UFUNCTION()
	void SoftReset(class ASexMagnetCharacter* PlayerToSpawn = nullptr);

	void HardReset();

	UPROPERTY(EditDefaultsOnly)
	float RespawnOffset = 100.0f;

	UPROPERTY()
	class ASexMagnetCharacter* PlayerOne = nullptr;

	UPROPERTY()
	class ASexMagnetCharacter* PlayerTwo = nullptr;

	UPROPERTY()
	class ASMCheckPoint* MyCheckpoint = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera properties")
	TSubclassOf<class ASMCoopCamera> CoopCameraClass;

	UPROPERTY()
	class ASMCoopCamera* CoopCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera properties")
	bool bSinglePlayer = false;

	UPROPERTY(EditDefaultsOnly, Category = "Camera properties")
	bool bManualCamera = false;

	UPROPERTY(VisibleAnywhere, Category = "Current Checkpoint")
	int32 CurrentCheckpoint = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<class ASMCheckPoint*> CheckPoints;

protected:
	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void SetUpCamera();

	UFUNCTION(BlueprintCallable)
	void SetUpCheckpoints();

	bool bOtherPlayerWantsReset = false;

};