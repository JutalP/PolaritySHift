#include "SMBeamVFXSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "SMFieldVFXActor.h"

USMBeamVFXSystem::USMBeamVFXSystem(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer){

}

void USMBeamVFXSystem::InitializeBeam(){
	FActorSpawnParameters Params;
	if(PositiveFieldClass)
		PositiveField = GetWorld()->SpawnActor<ASMFieldVFXActor>(
			PositiveFieldClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
			);

	if(NegativeFieldClass)
		NegativeField = GetWorld()->SpawnActor<ASMFieldVFXActor>(
			NegativeFieldClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
			);

	float ScaleValue = FieldRadius / 50;
	FVector FieldWorldScale = FVector(ScaleValue);

	if(PositiveField && NegativeField){
		PositiveField->SetActorHiddenInGame(true);
		PositiveField->SetActorScale3D(FieldWorldScale);
		NegativeField->SetActorHiddenInGame(true);
		NegativeField->SetActorScale3D(FieldWorldScale);
	}
}

void USMBeamVFXSystem::EnableBeam(const FVector& StartLocation, bool bPositive){
	if(bBeamSpawned){
		return;
	}

	bBeamSpawned = true;
	if(bPositive && PositiveField){
		CurrentField = PositiveField;
		BeamVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PositiveBeamVFX, StartLocation);
		PositiveField->SetActorHiddenInGame(false);
	} else if(NegativeField){
		CurrentField = NegativeField;
		BeamVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NegativeBeamVFX, StartLocation);
		NegativeField->SetActorHiddenInGame(false);
	}
}

void USMBeamVFXSystem::DisableBeam(){
	if(!bBeamSpawned || !BeamVFX){
		return;
	}

	CurrentField = nullptr;
	bBeamSpawned = false;
	BeamVFX->KillParticlesForced();
	BeamVFX->SetEmitterEnable(TEXT("Lightninglaser"), false);
	BeamVFX->SetEmitterEnable(TEXT("Innerlaser"), false);
	BeamVFX = nullptr;
	if(PositiveField){
		PositiveField->SetActorHiddenInGame(true);
	}
	if(NegativeField){
		NegativeField->SetActorHiddenInGame(true);
	}
}

void USMBeamVFXSystem::UpdateBeam(AActor* SourceActor, const FVector& Direction, float Distance){
	if(!CurrentField){
		return;
	}

	BeamVFX->bHiddenInGame = false;
	FVector EndLocation = SourceActor->GetActorLocation() + Direction * Distance;
	CurrentField->SetActorLocation(EndLocation);
	if(BeamVFX){
		BeamVFX->SetActorParameter("BeamSource", SourceActor);
		BeamVFX->SetActorParameter("BeamTarget", CurrentField);
	}
}