// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/CSWGameMode.h"

#include "BDThor.h"
#include "Kratos.h"
#include "Blueprint/UserWidget.h"
#include "CSW/AwakenThor.h"
#include "CSW/OutGameWidget.h"
#include "CSW/InGameWidget.h"

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
	
	OutGameWidget = CreateWidget<UOutGameWidget>(GetWorld(), WBP_GameStart);
	if (OutGameWidget)
			OutGameWidget->AddToViewport();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(OutGameWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);
	
		PlayerController->SetPause(true);
	}
	// bFirstPhase = false; // TMP!!
	if (bFirstPhase)
	{
		StartFirstPhase();
	}
	else
	{
		StartSecondPhase();
	}
}

void ACSWGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AKratos* player;
	APlayerController* PlayerController;
	
	if ((PlayerController = GetWorld()->GetFirstPlayerController()) != nullptr &&
		(player = Cast<AKratos>(PlayerController->GetPawn())) != nullptr)
	{
		SetPlayerHpBar(player->CurHP /  player->MaxHP);
		if (player->CurHP <= 90 && !bEndScreenStart)
		{
			FTimerHandle timer;
			GetWorld()->GetTimerManager().SetTimer(timer, [this]() {this->EndWithFail();}, 2.f, false);
			bEndScreenStart = true;
		}
		else
		{
			bEndScreenStart = false;
		}
	}
}

void ACSWGameMode::SetEnemyHpBar(float Percent)
{
	InGameWidget->SetEnemyHpBar(Percent);
}

void ACSWGameMode::SetPlayerHpBar(float Percent)
{
	InGameWidget->SetPlayerHpBar(Percent);
}

void ACSWGameMode::StartFirstPhase()
{
	AKratos* player;
	APlayerController* PlayerController;
	
	if ((PlayerController = GetWorld()->GetFirstPlayerController()) != nullptr &&
		(player = Cast<AKratos>(PlayerController->GetPawn())) != nullptr)
	{
		SetPlayerHpBar(player->CurHP /  player->MaxHP);
	}
	GetWorld()->SpawnActor<ABDThor>(BP_BDThor, FVector::ZeroVector, FRotator::ZeroRotator);
	InGameWidget->SetThorName(true);
}

void ACSWGameMode::StartSecondPhase()
{
	GetWorld()->SpawnActor<AAwakenThor>(BP_AwakenThor);
	InGameWidget->SetThorName(false);
}

void ACSWGameMode::GoToNextPhase()
{
	StartSecondPhase();
}

void ACSWGameMode::EndWithFail()
{
	OutGameWidget = CreateWidget<UOutGameWidget>(GetWorld(), WBP_GameFail);
	if (OutGameWidget)
		OutGameWidget->AddToViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(OutGameWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);

		PlayerController->SetPause(true);
	}
	if (bFirstPhase)
	{
		StartFirstPhase();
	}
	else
	{
		StartSecondPhase();
	}
}

void ACSWGameMode::EndWithSucceed()
{
	EndGameWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_GameSucceed);
	if (EndGameWidget)
	{
		EndGameWidget->AddToViewport();
	}
}
