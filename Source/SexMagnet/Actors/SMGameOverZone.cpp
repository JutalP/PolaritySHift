// Fill out your copyright notice in the Description page of Project Settings.

#include "SMGameOverZone.h"
#include "Components/BoxComponent.h"
#include "SexMagnetCharacter.h"

// Sets default values
ASMGameOverZone::ASMGameOverZone() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASMGameOverZone::GameOver);
}

// Called when the game starts or when spawned
void ASMGameOverZone::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ASMGameOverZone::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ASMGameOverZone::GameOver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA<ASexMagnetCharacter>()) {
		GameOverScreen();
		OtherActor->DisableInput(nullptr);
	}
}

