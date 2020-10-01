#include "SMFieldVFXActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"

ASMFieldVFXActor::ASMFieldVFXActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SphereMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SphereMeshComponent"));
	SphereMesh->SetupAttachment(RootComponent);
}

void ASMFieldVFXActor::SetRadius(float Radius) {
	float ScaleValue = Radius / 50;
	FVector MeshScale = FVector(ScaleValue);

	SphereMesh->SetWorldScale3D(MeshScale);
}