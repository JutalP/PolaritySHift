// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SexMagnetPlayer2.generated.h"

UCLASS()
class SEXMAGNET_API ASexMagnetPlayer2 : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASexMagnetPlayer2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
