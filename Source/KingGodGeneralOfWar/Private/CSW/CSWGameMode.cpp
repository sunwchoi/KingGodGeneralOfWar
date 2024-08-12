// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/CSWGameMode.h"

#include "BDThor.h"
#include "Kratos.h"
#include "Blueprint/UserWidget.h"
#include "CSW/AwakenThor.h"
#include "CSW/OutGameWidget.h"
#include "CSW/InGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ACSWGameMode::ACSWGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
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
	AudioComp->SetSound(IntroSound);
	AudioComp->Play();
	
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

void ACSWGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AKratos* player;
	APlayerController* PlayerController;
	
	if ((PlayerController = GetWorld()->GetFirstPlayerController()) != nullptr &&
		(player = Cast<AKratos>(PlayerController->GetPawn())) != nullptr)
	{
		SetPlayerHpBar(player->CurHP /  player->MaxHP);
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
	GetWorld()->SpawnActor<ABDThor>(BP_BDThor, FVector(0, 0, 800), FRotator::ZeroRotator);
	InGameWidget->SetThorName(true);
	AudioComp->SetSound(Phase1Sound);
	AudioComp->Play();
}

void ACSWGameMode::StartSecondPhase()
{
	AudioComp->Stop();
	AudioComp->SetSound(Phase2Sound);
	AudioComp->Play();
	GetWorld()->SpawnActor<AAwakenThor>(BP_AwakenThor, FVector(0, 0, 800), FRotator::ZeroRotator);
	InGameWidget->SetThorName(false);
	UGameplayStatics::PlaySound2D(GetWorld(), Phase2Sound);	
}

void ACSWGameMode::GoToNextPhase()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(
			handle,
			[this, PlayerController]()
			{
				StartSecondPhase();
			},
			5.f,
			false
			);
	}
	MiddleScene = CreateWidget<>(GetWorld(), WBP_MiddleScene);
	if (MiddleScene)
		MiddleScene->AddToViewport();
}

void ACSWGameMode::EndWithFail()
{
	OutGameWidget = CreateWidget<UOutGameWidget>(GetWorld(), WBP_GameFail);
	if (OutGameWidget)
		OutGameWidget->AddToViewport();
}

void ACSWGameMode::EndWithSucceed()
{
	EndGameWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_GameSucceed);
	if (EndGameWidget)
	{
		EndGameWidget->AddToViewport();
	}

	AudioComp->Stop();
	AudioComp->SetSound(EndingSound);
	AudioComp->Play();
}

