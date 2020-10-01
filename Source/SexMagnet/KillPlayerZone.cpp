// Fill out your copyright notice in the Description page of Project Settings.

#include "KillPlayerZone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SexMagnetCharacter.h"
#include "SexMagnetGameMode.h"
#include "GameFramework/Actor.h"

// Sets default values
AKillPlayerZone::AKillPlayerZone() {

	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AKillPlayerZone::OnOverlap);
}

// Called when the game starts or when spawned
void AKillPlayerZone::BeginPlay() {
	Super::BeginPlay();

}

void AKillPlayerZone::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA<ASexMagnetCharacter>()) {
		ASexMagnetCharacter* MyPlayer = Cast<ASexMagnetCharacter>(OtherActor);
		ASexMagnetGameMode* MyGameMode = Cast<ASexMagnetGameMode>(GetWorld()->GetAuthGameMode());
		MyGameMode->SoftReset(MyPlayer);
	}

}

// Called every frame
void AKillPlayerZone::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

