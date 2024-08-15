// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/CSWGameMode.h"

#include "BDThor.h"
#include "BDThorFSM.h"
#include "Kratos.h"
#include "Blueprint/UserWidget.h"
#include "CSW/AwakenThor.h"
#include "CSW/OutGameWidget.h"
#include "CSW/InGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "MovieSceneSequencePlaybackSettings.h"

#include <Runtime/LevelSequence/Public/LevelSequencePlayer.h>

#include "CSW/HitWidget.h"

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
	
	HitWidget = CreateWidget<UHitWidget>(GetWorld(), WBP_Hit);
	if (HitWidget)
		HitWidget->AddToViewport();
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
	
}

void ACSWGameMode::SetEnemyHpBar(float Percent)
{
	InGameWidget->SetEnemyHpBar(Percent);
}

void ACSWGameMode::SetPlayerHpBar(float Percent)
{
	InGameWidget->SetPlayerHpBar(Percent);
}

void ACSWGameMode::PlayHitWidgetAnim()
{
	HitWidget->PlayHitAnimation();
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
	UE_LOG(LogTemp, Warning, TEXT("Goto"));
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
	OutGameWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_GameFail);
	if (OutGameWidget)
		OutGameWidget->AddToViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(OutGameWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);
	}
}

void ACSWGameMode::EndWithSucceed()
{
	if (SQ_FinalScene)
	{
		ALevelSequenceActor* outActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SQ_FinalScene, FMovieSceneSequencePlaybackSettings(), outActor);
		SequencePlayer->Play();
	}

	//EndGameWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_GameSucceed);
	//if (EndGameWidget)
	//{
	//	EndGameWidget->AddToViewport();
	//}

	//AudioComp->Stop();
	//AudioComp->SetSound(EndingSound);
	//AudioComp->Play();
}

void ACSWGameMode::EndFirstThor()
{
	ABDThor* BDThor = Cast<ABDThor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABDThor::StaticClass()));

	if (BDThor->fsm->BDCurrentHP <= 140.0f) {
		if (SQ_middleScene)
		{
			ALevelSequenceActor* outActor;
			ULevelSequencePlayer* BDSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SQ_middleScene, FMovieSceneSequencePlaybackSettings(), outActor);
			
			// ������ ����� ������ GoToNextPhase�� ȣ���ϵ��� ���ε�
			//BDSequencePlayer->OnFinished.AddDynamic(this, &ACSWGameMode::GoToNextPhase);

			if (BDSequencePlayer)
			{
				BDSequencePlayer->Play();
				FTimerHandle handle;
				// �������� ���� �� �Լ��� ȣ���ϵ��� Ÿ�̸� ���� (��: 10�� ��)
				GetWorld()->GetTimerManager().SetTimer(
					handle,
					this,
					&ACSWGameMode::GoToNextPhase,
					20.f, // �������� ��� �ð��� ���⿡ �ش��ϴ� �ð����� ����
					false
				);
			}
			
		}
	}
}


