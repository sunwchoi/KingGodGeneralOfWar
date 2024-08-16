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

#include "Axe.h"
#include "CSW/HitWidget.h"

ACSWGameMode::ACSWGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	AmbientAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbientAudioComp"));
	AmbientAudioComp->SetupAttachment(RootComponent);
}

void ACSWGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}

	InGameWidget = CreateWidget<UInGameWidget>(GetWorld(), WBP_InGame);
	if (InGameWidget)
		InGameWidget->AddToViewport();
	
	HitWidget = CreateWidget<UHitWidget>(GetWorld(), WBP_Hit);
	if (HitWidget)
		HitWidget->AddToViewport();
	
	StartFirstPhase();
	//StartSecondPhase();
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

void ACSWGameMode::PlayHpUIFadeInAnim()
{
	InGameWidget->PlayFadeInAnim();
}

void ACSWGameMode::PlayHpUIFadeOutAnim()
{
	InGameWidget->PlayFadeOutAnim();	
}

void ACSWGameMode::StartFirstPhase()
{
	GetWorld()->SpawnActor<ABDThor>(BP_BDThor, FVector(0, 0, 800), FRotator::ZeroRotator);
	InGameWidget->SetThorName(true);
	AudioComp->SetSound(Phase1Sound);
	AudioComp->Play();
	AmbientAudioComp->SetSound(Phase1AmbientSound);
	AmbientAudioComp->Play();
}

void ACSWGameMode::StartSecondPhase()
{
	AudioComp->SetSound(Phase2Sound);
	AudioComp->Play();
	AmbientAudioComp->SetSound(Phase2AmbientSound);
	AmbientAudioComp->Play();
	Thor2 = GetWorld()->SpawnActor<AAwakenThor>(BP_AwakenThor, FVector(0, 0, 800), FRotator::ZeroRotator);
	InGameWidget->SetThorName(false);
}

void ACSWGameMode::GoToNextPhase()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		StartSecondPhase();
	}
}

void ACSWGameMode::EndWithFail()
{
	EndGameWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_GameFail);
	if (EndGameWidget)
		EndGameWidget->AddToViewport();
	 
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(EndGameWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);
	}
}

void ACSWGameMode::EndWithSucceed()
{
	// 페이드인
	if (SQ_FinalFadeIn)
	{
		
		
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("SQ_FinalFadeIn")));
		ALevelSequenceActor* outActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SQ_FinalFadeIn, FMovieSceneSequencePlaybackSettings(), outActor);
		SequencePlayer->Play();
		SequencePlayer->OnFinished.AddDynamic(this, &ACSWGameMode::PlayFinalSequence);
	}
	
}

void ACSWGameMode::EndFirstThor()
{
	ABDThor* BDThor = Cast<ABDThor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABDThor::StaticClass()));

	if (BDThor->fsm->BDCurrentHP <= 50.0f) {
		AudioComp->Stop();
		AmbientAudioComp->Stop();
		if (SQ_middleScene)
		{
			ALevelSequenceActor* outActor;
			ULevelSequencePlayer* BDSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SQ_middleScene, FMovieSceneSequencePlaybackSettings(), outActor);
			
			// ������ ����� ������ GoToNextPhase�� ȣ���ϵ��� ���ε�
			//BDSequencePlayer->OnFinished.AddDynamic(this, &ACSWGameMode::GoToNextPhase);

			if (BDSequencePlayer)
			{
				BDSequencePlayer->Play();
				FTimerHandle tmp0;
				GetWorld()->GetTimerManager().SetTimer(tmp0, []()
				{
						
				},
				2.f, false);
				FTimerHandle handle;
				// �������� ���� �� �Լ��� ȣ���ϵ��� Ÿ�̸� ���� (��: 10�� ��)
				GetWorld()->GetTimerManager().SetTimer(
					handle,
					this,
					&ACSWGameMode::GoToNextPhase,
					23.f, // �������� ��� �ð��� ���⿡ �ش��ϴ� �ð����� ����
					false
				);
			}
			
		}
	}
}

void ACSWGameMode::PlayFinalSequence()
{
	if(SQ_FinalScene)
	{
		AKratos* player = Cast<AKratos>(GetWorld()->GetFirstPlayerController()->GetPawn());

		player->SetActorHiddenInGame(true);
		player->Axe->SetActorHiddenInGame(true);
		Thor2->SetActorHiddenInGame(true);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("SQ_FinalScene")));
		ALevelSequenceActor* outActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SQ_FinalScene, FMovieSceneSequencePlaybackSettings(), outActor);
		SequencePlayer->Play();
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ACSWGameMode::EndGame, 8.0f);
		//
	}
}

void ACSWGameMode::EndGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("EndGame")));
	EndGameWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_GameSucceed);
	if (EndGameWidget)
	{
		EndGameWidget->AddToViewport();
	}

	AudioComp->Stop();
	AmbientAudioComp->Stop();
	AudioComp->SetSound(EndingSound);
	AudioComp->Play();
}


