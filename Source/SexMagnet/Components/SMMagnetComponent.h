#pragma once

#include "Components/ActorComponent.h"
#include "SMMagnetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnStayPositiveField, AActor*, Instigator, float, Force);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnStayNegativeField, AActor*, Instigator, float, Force);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnCharacterStayPositiveField, class ASexMagnetCharacter*, Character, const FVector&, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnCharacterStayNegativeField, class ASexMagnetCharacter*, Character, const FVector&, Direction);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnCharacterStayPositiveBeam, class ASexMagnetCharacter*, Character, const FVector&, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnCharacterStayNegativeBeam, class ASexMagnetCharacter*, Character, const FVector&, Direction);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEXMAGNET_API USMMagnetComponent : public UActorComponent {
	GENERATED_BODY()

public:
	USMMagnetComponent(const FObjectInitializer& ObjectInitializer){}

	UFUNCTION()
	void EnableMagnetism();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyPositiveField(class ASexMagnetCharacter* Instigator, const FVector& Direction, float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyNegativeField(class ASexMagnetCharacter* Instigator, const FVector& Direction, float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotitfyPositiveBeam(class ASexMagnetCharacter* Instigator, const FVector& Direction, float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyNegativeBeam(class ASexMagnetCharacter* Instigator, const FVector& Direction, float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsPositive(bool Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsNegative(bool Value);
	
	UFUNCTION(BlueprintCallable)
	void SetMovableComponent(class USMMovableComponent* Movable);

	UPROPERTY()
	bool bIsDisrupted = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Magnetism)
	bool bIsPositive = false;

	UPROPERTY(BlueprintReadOnly, Category = Magnetism)
	bool bIsNegative = false;

	class USMMovableComponent* SMMovable = nullptr;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnStayPositiveField OnStayPositiveField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnStayNegativeField OnStayNegativeField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnCharacterStayPositiveField OnCharacterStayPositiveField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnCharacterStayNegativeField OnCharacterStayNegativeField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnCharacterStayPositiveBeam OnCharacterStayPositiveBeam;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnCharacterStayNegativeBeam OnCharacterStayNegativeBeam;
};