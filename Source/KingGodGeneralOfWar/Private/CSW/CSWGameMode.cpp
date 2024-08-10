// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/CSWGameMode.h"

#include "BDThor.h"
#include "Kratos.h"
#include "Blueprint/UserWidget.h"
#include "CSW/AwakenThor.h"
#include "CSW/OutGameWidget.h"
#include "CSW/InGameWidget.h"
#include "Kismet/GameplayStatics.h"

ACSWGameMode::ACSWGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ACSWGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	InGameWidget = CreateWidget<UInGameWidget>(GetWorld(), WBP_InGame);
	if (InGameWidget)
		InGameWidget->AddToViewport();
	
	GameStartWidget = CreateWidget<UOutGameWidget>(GetWorld(), WBP_GameStart);
	if (GameStartWidget)
			GameStartWidget->AddToViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(GameStartWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);

		PlayerController->SetPause(true);
	}
	StartSecondPhase();
}

void ACSWGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AKratos* player;
	APlayerController* PlayerController;
	
	if ((PlayerController = GetWorld()->GetFirstPlayerController()) != nullptr &&
		(player = Cast<AKratos>(PlayerController->GetPawn())) != nullptr)
	{
		InGameWidget->SetPlayerHpBar(player->CurHP /  player->MaxHP);
	}
	
	
}

void ACSWGameMode::SetEnemyHpBar(float Percent)
{
	InGameWidget->SetEnemyHpBar(Percent);
}

void ACSWGameMode::SetPlayerHpBar(float Percent)
{
}

void ACSWGameMode::StartFirstPhase()
{
	GetWorld()->SpawnActor<ABDThor>(BP_BDThor, FVector::ZeroVector, FRotator::ZeroRotator);
	InGameWidget->SetThorName(true);
}

void ACSWGameMode::StartSecondPhase()
{
	AActor* tmp = GetWorld()->SpawnActor<AAwakenThor>(BP_AwakenThor);
	ACharacter* tmpC = Cast<ACharacter>(tmp);

	if (tmpC != nullptr)
	{
		FVector loc = tmpC->GetActorLocation(); 
		tmpC->SetActorLocation(FVector(loc.X, loc.Y, 100));
	}
	
	InGameWidget->SetThorName(false);
}
