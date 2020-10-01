#include "SMCheckPoint.h"
#include "Components/BoxComponent.h"
#include "SexMagnetCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "SexMagnetGameMode.h"
#include "Components/PrimitiveComponent.h"

ASMCheckPoint::ASMCheckPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASMCheckPoint::OnOverlap);
}

void ASMCheckPoint::BeginPlay() {
	Super::BeginPlay();


	OnRespawnDelegate.AddDynamic(this, &ASMCheckPoint::OnRespawn);
}

void ASMCheckPoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (bReached) {
		return;
	}
	if (OtherActor->IsA<ASexMagnetCharacter>()) {
		ASexMagnetGameMode* MyGamemode = Cast<ASexMagnetGameMode>(GetWorld()->GetAuthGameMode());
		MyGamemode->SetCurrentCheckpoint(Index);
		bIsActivated = true;
		/*if (OnCheckpointReached.IsBound()) {
			OnCheckpointReached.Broadcast(Index);
		}*/
		SpawnEmitter();
	}
	bReached = true;
}