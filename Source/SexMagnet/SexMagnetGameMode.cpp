#include "SexMagnetGameMode.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SexMagnetCharacter.h"
#include "Camera/SMCoopCamera.h"
#include "Engine/World.h"
#include "Actors/SMCheckPoint.h"

ASexMagnetGameMode::ASexMagnetGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASexMagnetGameMode::BeginPlay() {
	Super::BeginPlay();
	SetUpCamera();
	SetUpCheckpoints();
	// Dangerous, reliant on being called after SetUpCamera
	PlayerOne->OnRequestReset.AddDynamic(this, &ASexMagnetGameMode::SoftReset);
	PlayerTwo->OnRequestReset.AddDynamic(this, &ASexMagnetGameMode::SoftReset);
}

void ASexMagnetGameMode::Tick(float DeltaSeconds) {
	if (CoopCamera) {
		FVector SecondPlayer = bSinglePlayer ? PlayerOne->GetActorLocation() : PlayerTwo->GetActorLocation();
		CoopCamera->Update(DeltaSeconds, PlayerOne->GetActorLocation(), SecondPlayer);
	}
}

void ASexMagnetGameMode::SetUpCamera() {
	PlayerOne = Cast<ASexMagnetCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1));
	PlayerTwo = Cast<ASexMagnetCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerOne) {
		return;
	}

	if (CoopCameraClass) {
		CoopCamera = GetWorld()->SpawnActor<ASMCoopCamera>(CoopCameraClass);
	}
	if (!CoopCamera) {
		return;
	}
	PlayerOne->OnCharacterLanded.AddDynamic(CoopCamera, &ASMCoopCamera::ToggleUpdateZOffset);
	if (!bSinglePlayer) {
		PlayerTwo->OnCharacterLanded.AddDynamic(CoopCamera, &ASMCoopCamera::ToggleUpdateZOffset);
	}
	CoopCamera->Initialize(PlayerOne, PlayerTwo);
}

void ASexMagnetGameMode::SetUpCheckpoints() {
	CheckPoints.Empty();
	TArray<AActor*> FoundCheckPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASMCheckPoint::StaticClass(), FoundCheckPoints);
	UE_LOG(LogTemp, Warning, TEXT("[GameMode] FoundCheckPoints: %d"), FoundCheckPoints.Num());
	for (AActor* A : FoundCheckPoints) {
		ASMCheckPoint* ACP = Cast<ASMCheckPoint>(A);
		//if (!CheckPoints.Contains(ACP)) {
			CheckPoints.Add(ACP);
			if (!ACP->OnCheckpointReached.IsBound())
			{
				UE_LOG(LogTemp, Warning, TEXT("[GameMode] Checkpoint index: %d"), ACP->Index);
				ACP->OnCheckpointReached.AddDynamic(this, &ASexMagnetGameMode::SetCurrentCheckpoint);
			}
		//}
	}

	CheckPoints.Sort();

	UE_LOG(LogTemp, Warning, TEXT("[GameMode] CheckPoints: %d"), CheckPoints.Num());
	for (ASMCheckPoint* CP : CheckPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameMode] CheckPoint index: %d"), CP->Index);
	}

	/*CheckPoints.Sort([](const ASMCheckPoint& A, const ASMCheckPoint& B)
	{
		int32 aInt = FCString::Atoi(*A.GetName().Right(2));
		int32 bInt = FCString::Atoi(*B.GetName().Right(2));

		UE_LOG(LogTemp, Error, TEXT("A: %d    B: %d"), aInt, bInt);
		return aInt > bInt;
	});*/
	/*for (AActor* A : CheckPoints) {
		UE_LOG(LogTemp, Error, TEXT("%s"), *A->GetName());
	}*/
}

void ASexMagnetGameMode::SetCurrentCheckpoint(int32 Index) {
	if (Index > CurrentCheckpoint) {
		CurrentCheckpoint = Index;
		UE_LOG(LogTemp, Error, TEXT("Set checkpoint to %d"), CurrentCheckpoint);
	}
}

void ASexMagnetGameMode::SoftReset(ASexMagnetCharacter* PlayerToSpawn /*= nullptr*/) {
	if (CheckPoints.Num() == 0) {
		return;
	}

	if (PlayerOne->bIsPaused || PlayerTwo->bIsPaused) {
		HardReset();
		return;
	}

	float CorrectIndex = CurrentCheckpoint;
	FString CPName = CheckPoints[CorrectIndex]->GetName();

	UE_LOG(LogTemp, Warning, TEXT("Respawning checkpoint: %s index %d"), *CPName, CorrectIndex);
	UE_LOG(LogTemp, Warning, TEXT("[GameMode] Checkpoint num: %d"), CheckPoints.Num());

	if (PlayerToSpawn) {
		PlayerToSpawn->SetActorLocation(CheckPoints[CorrectIndex]->GetActorLocation());
	}
	else {
		//TODO: Can be a problem to spawn two character in the same position

		FVector OffsetVector = FVector(0.0f, RespawnOffset, 0.0f);
		PlayerOne->SetActorLocation(CheckPoints[CorrectIndex]->GetActorLocation() + OffsetVector);
		PlayerTwo->SetActorLocation(CheckPoints[CorrectIndex]->GetActorLocation() - OffsetVector);
	}
	CheckPoints[CorrectIndex]->OnRespawnDelegate.Broadcast();
	CheckPoints[CorrectIndex]->SpawnEmitter();
}

void ASexMagnetGameMode::HardReset() {
	CurrentCheckpoint = 0;
	UGameplayStatics::OpenLevel(GetWorld(), "?restart");
}