#include "SMCoopCamera.h"
#include "SexMagnetCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

ASMCoopCamera::ASMCoopCamera(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer){
	SpringArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("Boom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 150.f);
	SpringArm->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	//Create a camera and attach to boom
	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CoopCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void ASMCoopCamera::Initialize(ASexMagnetCharacter* PlayerOne, ASexMagnetCharacter* PlayerTwo /*= nullptr*/){
	APlayerController* POC = Cast<APlayerController>(PlayerOne->GetController());
	POC->SetViewTargetWithBlend(this);
	if(PlayerTwo != nullptr){
		APlayerController* PTC = Cast<APlayerController>(PlayerTwo->GetController());
		PTC->SetViewTargetWithBlend(this);
	}
}

void ASMCoopCamera::Update(float DeltaSeconds, const FVector& FirstPos, const FVector& SecondPos){
	FBox Box = FBox(FirstPos, SecondPos);
	CurrentLowestZ = FirstPos.Z > SecondPos.Z ? SecondPos.Z : FirstPos.Z;

	float PositionX = Box.GetCenter().X;
	float PositionY = SpringArm->GetComponentLocation().Y;
	float PositionZ = SpringArm->GetComponentLocation().Z;

	if (Box.GetCenter().Z - CurrentLowestZ > IncreaseSpeedZThreshold) {
		AccomodateFastFallers();
	} else {
		TmpVerticalMoveSpeed = VerticalMoveSpeed;
	}

	FVector NewLocation = FVector(PositionX, PositionY, PositionZ);
	UpdateVerticalPosition(DeltaSeconds, NewLocation, Box.GetCenter().Z + CameraZOffset);
	UpdateHorizontalPosition(DeltaSeconds, NewLocation, Box.GetCenter().Y);
	SpringArm->SetWorldLocation(NewLocation);

	if(bDrawDebug){
		DrawDebugBox(GetWorld(), Box.GetCenter(), Box.GetExtent(), FQuat::Identity, FColor::White, false, -1.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), Box.GetCenter(), 10.0f, 32, FColor::White);

		FVector LowestPos = FirstPos.Z > SecondPos.Z ? SecondPos : FirstPos;
		FVector LowestCameraPos = LowestPos + FVector::UpVector * MaximumVerticalOffset;
		DrawDebugLine(GetWorld(), LowestPos, LowestCameraPos, FColor::Red, false, -1.0f, 0, 5.0f);

		FColor HeightLineColor = Box.GetCenter().Z > LowestCameraPos.Z ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), LowestCameraPos, Box.GetCenter(), HeightLineColor, false, -1.0f, 0, 5.0f);
	}
}

void ASMCoopCamera::ToggleUpdateZOffset(){
	ZLerpAlpha = 0.0f;
}

void ASMCoopCamera::UpdateVerticalPosition(float DeltaSeconds, FVector& OutLocation, float NewHeight){
	float MaxCameraHeight = CurrentLowestZ + MaximumVerticalOffset;
	float OldPositionZ = SpringArm->GetComponentLocation().Z;
	float NewPositionZ = NewHeight < MaxCameraHeight ? NewHeight : MaxCameraHeight;
	if(bAdjustHeightOnLanding && ZLerpAlpha < 1){
		ZLerpAlpha += VerticalMoveSpeed * DeltaSeconds;
		OutLocation.Z = FMath::LerpStable(OldPositionZ, NewPositionZ, TmpVerticalMoveSpeed * DeltaSeconds);
	} else{
		OutLocation.Z = FMath::LerpStable(OldPositionZ, NewPositionZ, TmpVerticalMoveSpeed * DeltaSeconds);
	}
}

void ASMCoopCamera::UpdateHorizontalPosition(float DeltaSeconds, FVector& OutLocation, float NewPositionY){
	OutLocation.Y = FMath::LerpStable(OutLocation.Y, NewPositionY, HorizontalMoveSpeed * DeltaSeconds);
}

void ASMCoopCamera::AccomodateFastFallers() {
	TmpVerticalMoveSpeed = VerticalMoveSpeed * VerticalMoveSpeedIncreaseMultiplier;
}
