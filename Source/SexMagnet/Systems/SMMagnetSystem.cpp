#include "SMMagnetSystem.h"
#include "WorldCollision.h"
#include "Engine/CollisionProfile.h"
#include "Components/SMMagnetComponent.h"
#include "SexMagnetCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/SphereComponent.h"
#include "VFX/SMFieldVFXActor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "VFX/SMBeamVFXSystem.h"
#include "Components/AudioComponent.h"
#include "Components/SMMovableComponent.h"
#include "Actors/SMPuzzleCube.h"

USMMagnetSystem::USMMagnetSystem(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer){
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	LookDirection = FVector::ZeroVector;
}

void USMMagnetSystem::BeginPlay(){
	Super::BeginPlay();
	CachedCollisionShape = FCollisionShape::MakeSphere(FieldRadius);
	CachedCollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(SphereOverlapComponents), false, GetOwner());

	for(auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter){
		const ECollisionChannel& Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		CachedCollisionObjectParams.AddObjectTypesToQuery(Channel);
	}
}

void USMMagnetSystem::Initialize(AActor* InActor){
	OwnerActor = InActor;
	ensure(OwnerActor != nullptr);

	OwnerCharacter = Cast<ASexMagnetCharacter>(OwnerActor);
	ensure(OwnerCharacter != nullptr);

	OwnerMagnet = Cast<USMMagnetComponent>(OwnerActor->GetComponentByClass(USMMagnetComponent::StaticClass()));
	ensure(OwnerMagnet != nullptr);

	FActorSpawnParameters Params;
	PositiveField = GetWorld()->SpawnActor<ASMFieldVFXActor>(
		PositiveFieldClass,
		OwnerActor->GetActorLocation(),
		OwnerActor->GetActorRotation(),
		Params
		);

	NegativeField = GetWorld()->SpawnActor<ASMFieldVFXActor>(
		NegativeFieldClass,
		OwnerActor->GetActorLocation(),
		OwnerActor->GetActorRotation(),
		Params
		);

	float ScaleValue = FieldRadius / 50;
	FVector FieldWorldScale = FVector(ScaleValue);
	PositiveField->SetActorScale3D(FieldWorldScale);
	NegativeField->SetActorScale3D(FieldWorldScale);
	DisableFields();

	BeamVFXSystem = NewObject<USMBeamVFXSystem>(this, BeamVFXSystemClass);
	ensure(BeamVFXSystem != nullptr);
	BeamVFXSystem->InitializeBeam();
}

void USMMagnetSystem::EngagePositiveField(float Value){
	if(Value > KINDA_SMALL_NUMBER){
		bPositiveCharge = true;
		EngageField(Value);
	} else if(bPositiveCharge){
		bPositiveCharge = false;
		UnEngageField();
	}
}

void USMMagnetSystem::EngageNegativeField(float Value){
	if(Value > KINDA_SMALL_NUMBER){
		bNegativeCharge = true;
		EngageField(Value);
	} else if(bNegativeCharge){
		bNegativeCharge = false;
		UnEngageField();
	}
}

void USMMagnetSystem::EngageField(float Value){
	SetOwnerMagnetPol();
	DisableFields();
	if(LookDirectionY != 0 || LookDirectionZ != 0){
		OwnerCharacter->GetMovementComponent()->SetComponentTickEnabled(true);
		DirectField();
		EngageBeam(Value);
		bPositivePrevFrame = bPositiveCharge;
		return;
	}

	if(RadiusLerpAlpha < 1){
		RadiusLerpAlpha += FieldGrowthRate * FApp::GetDeltaTime();
		TmpRadius = FMath::Lerp(MinimumFieldRadius, FieldRadius, RadiusLerpAlpha);
	} else{
		TmpRadius = FieldRadius;
	}

	SetFieldRadius(TmpRadius);
	EnableField();
	TArray<USMMagnetComponent*> FoundMagnets;
	GetMagnetsInRadius(FoundMagnets, FieldRadius);
	GetMagnetsAlongLine(FoundMagnets, -OwnerActor->GetActorUpVector(), TraceDistance);

	for(USMMagnetComponent* Magnet : FoundMagnets){
		if (Magnet) {
			if(bPositiveCharge){
				Magnet->NotifyPositiveField(OwnerCharacter, OwnerCharacter->MoveDirection, Value);
			} else if(bNegativeCharge){
				Magnet->NotifyNegativeField(OwnerCharacter, OwnerCharacter->MoveDirection, Value);
			}
		}
	}
}

void USMMagnetSystem::UnEngageField(){
	bPositiveCharge = false;
	bNegativeCharge = false;
	ASexMagnetCharacter* MyPlayer = Cast<ASexMagnetCharacter>(GetOwner());
	MyPlayer->bIsPlaying = false;
	SetOwnerMagnetPol();
	ClearParameters();
}

void USMMagnetSystem::SetFieldRadius(float Value){
	if(bPositiveCharge){
		PositiveField->SetRadius(TmpRadius);
	} else if(bNegativeCharge){
		NegativeField->SetRadius(TmpRadius);
	}
}

void USMMagnetSystem::SetOwnerMagnetPol(){
	OwnerMagnet->SetIsPositive(bPositiveCharge);
	OwnerMagnet->SetIsNegative(bNegativeCharge);
}

void USMMagnetSystem::EnableField(){
	if(bPositiveCharge){
		PositiveField->SetActorHiddenInGame(false);
		PositiveField->SetActorLocation(OwnerActor->GetActorLocation());
	} else if(bNegativeCharge){
		NegativeField->SetActorHiddenInGame(false);
		NegativeField->SetActorLocation(OwnerActor->GetActorLocation());
	}
}

void USMMagnetSystem::DisableFields(){
	PositiveField->SetActorHiddenInGame(true);
	NegativeField->SetActorHiddenInGame(true);
}

void USMMagnetSystem::ClearParameters(){
	if(!bPositiveCharge && !bNegativeCharge){
		TmpRadius = 0.0f;
		RadiusLerpAlpha = 0.0f;
		OwnerCharacter->GetMovementComponent()->SetComponentTickEnabled(true);
		DisableFields();
		StopDirectField();
	}
}

void USMMagnetSystem::DirectField(){
	if(bPositivePrevFrame && bPositiveCharge){
		bNegativeCharge = false;
	} else if(!bPositivePrevFrame && bNegativeCharge){
		bPositiveCharge = false;
	}
	SetOwnerMagnetPol();
	TmpBeamDistance = FMath::Lerp(TmpBeamDistance, BeamMaxDistance * LookDirection.Size(), GetWorld()->GetDeltaSeconds() * BeamGrowthRate);
	BeamVFXSystem->EnableBeam(OwnerActor->GetActorLocation(), bPositiveCharge);
	BeamVFXSystem->UpdateBeam(OwnerActor, LookDirection, TmpBeamDistance);
}

void USMMagnetSystem::StopDirectField(){
	BeamVFXSystem->DisableBeam();
}

void USMMagnetSystem::EngageBeam(float Value)
{
	TArray<USMMagnetComponent*> FoundMagnets;
	GetMagnetsAlongLine(FoundMagnets, LookDirection, TmpBeamDistance, true);

	if (Value < TriggerHeldThreshold) 
		return;

	for(USMMagnetComponent* Magnet : FoundMagnets){
		if(!Magnet){
			continue;
		}
		if(bPositiveCharge){
			if (Magnet->SMMovable->bCarriedNegative) {
				Magnet->SMMovable->Detach();
				Magnet->SMMovable->ResetCarryPolarity();
				bPositiveCharge = false;
				return;
			}
			Magnet->NotitfyPositiveBeam_Implementation(OwnerCharacter, OwnerCharacter->MoveDirection, TmpBeamDistance);
		} else if(bNegativeCharge){
			if (Magnet->SMMovable->bCarriedPositive) {
				Magnet->SMMovable->Detach();
				Magnet->SMMovable->ResetCarryPolarity();
				bNegativeCharge = false;
				return;
			}
			Magnet->NotifyNegativeBeam_Implementation(OwnerCharacter, OwnerCharacter->MoveDirection, TmpBeamDistance);
		}

		ASMPuzzleCube* Cube = Cast<ASMPuzzleCube>(Magnet->GetOwner());
		if (Cube) {
			OwnerCharacter->CarriedCube = Cube;
		}

		OwnerCharacter->OnDisengageBeam.AddUniqueDynamic(Magnet->SMMovable, &USMMovableComponent::Detach);
	}
}

void USMMagnetSystem::EngageDisruptionField(){
	UE_LOG(LogTemp, Warning, TEXT("[MagnetSystem] EngagedDisruptionField"));
}

void USMMagnetSystem::DisableDisruptionField(){
	OnDisableDisruption.Broadcast();
	OnDisableDisruption.RemoveAll(this);
}

void USMMagnetSystem::PointVertical(float Value){
	LookDirectionZ = -Value;
}

void USMMagnetSystem::PointHorizontal(float Value){
	LookDirectionY = -Value;
}

void USMMagnetSystem::Update(){
	if(LookDirectionY == 0 && LookDirectionZ == 0){
		StopDirectField();
		LookDirection.Normalize();
		LookDirection *= BeamMinDistance;
	}

	if(LookDirectionY != 0){
		LookDirection.Y = FMath::Lerp(LookDirection.Y, LookDirectionY, GetWorld()->GetDeltaSeconds() * BeamSpeed);
	}
	if(LookDirectionZ != 0){
		LookDirection.Z = FMath::Lerp(LookDirection.Z, LookDirectionZ, GetWorld()->GetDeltaSeconds() * BeamSpeed);
	}
	LookDirection = LookDirection.GetClampedToSize(BeamMinDistance, 1);
}

void USMMagnetSystem::GetMagnetsInRadius(TArray<USMMagnetComponent*>& OutMagnets, float Radius){
	CachedCollisionShape = FCollisionShape::MakeSphere(Radius);
	const FVector StartLocation = OwnerActor->GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams LineTraceCollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(LineTraceSingle), false);

	TArray<FOverlapResult> Overlaps;

	const bool bHit = GetWorld()->OverlapMultiByObjectType(Overlaps, StartLocation, FQuat::Identity, CachedCollisionObjectParams, CachedCollisionShape, CachedCollisionQueryParams);

	TArray<AActor*> TargetsInLineOfSight;
	for(FOverlapResult& O : Overlaps){
		if(!O.GetActor()->IsValidLowLevel())
			continue;

		if(O.GetActor() == OwnerActor)
			continue;

		for(TSubclassOf<AActor> ClassToIgnore : ActorsToIgnore){
			if(O.GetActor()->IsA(ClassToIgnore))
				continue;
		}

		if(!O.GetActor()->FindComponentByClass(USMMagnetComponent::StaticClass()))
			continue;

		const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, O.GetActor()->GetActorLocation(), ECC_Visibility, LineTraceCollisionQueryParams);
		AActor* FoundActor = Hit.GetActor();
		AActor* OverlappedActor = O.GetActor();

		if(!bHit || FoundActor == OverlappedActor)
			TargetsInLineOfSight.Add(OverlappedActor);

		for(AActor* AvailableTarget : TargetsInLineOfSight){
			USMMagnetComponent* MagComp = Cast<USMMagnetComponent>(AvailableTarget->GetComponentByClass(USMMagnetComponent::StaticClass()));
			if(!OutMagnets.Contains(MagComp)){
				OutMagnets.Add(MagComp);
			}
		}
	}
}

void USMMagnetSystem::GetMagnetsAlongLine(TArray<USMMagnetComponent*>& OutMagnets, const FVector& Direction, float Range, bool bDrawDebugLines /*= false*/){
	TmpBeamDistance = Range;
	const FVector StartLocation = OwnerActor->GetActorLocation();
	const FVector EndLocation = StartLocation + Direction * Range;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerActor);

	TArray<FHitResult> HitResults;

	GetWorld()->LineTraceMultiByObjectType(HitResults,
										   StartLocation,
										   EndLocation,
										   CachedCollisionObjectParams,
										   CachedCollisionQueryParams);

	for(FHitResult& Hit : HitResults){
		AActor* FoundActor = Hit.GetActor();
		if(FoundActor){
			USMMagnetComponent* MagComp = Cast<USMMagnetComponent>(FoundActor->GetComponentByClass(USMMagnetComponent::StaticClass()));
			if(!MagComp){
				continue;
			}
			if (MagComp->GetOwner()->IsA<ASexMagnetCharacter>() && OwnerCharacter->bIsCarried) {
				continue;
			}
			if(!OutMagnets.Contains(MagComp)){
				OutMagnets.Add(MagComp);
			}
		}
	}
}