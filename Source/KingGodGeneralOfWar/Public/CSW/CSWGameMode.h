// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CSWGameMode.generated.h"

/**
 * 
 */

UCLASS()
class KINGGODGENERALOFWAR_API ACSWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACSWGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SetEnemyHpBar(float Percent);
	void SetPlayerHpBar(float Percent);

	void PlayHitWidgetAnim();

	void StartFirstPhase();
	void StartSecondPhase();
	void GoToNextPhase();

	void EndWithFail();
	void EndWithSucceed();
	void EndFirstThor();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UOutGameWidget> WBP_GameStart;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameWidget> WBP_InGame;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHitWidget> WBP_Hit;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WBP_GameFail;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WBP_GameSucceed;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WBP_MiddleScene;

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AAwakenThor> BP_AwakenThor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABDThor> BP_BDThor;
	
	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* SQ_FinalScene;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* SQ_middleScene;

	UPROPERTY()
	UUserWidget* OutGameWidget;

	UPROPERTY()
	class UInGameWidget* InGameWidget;

	UPROPERTY()
	class UHitWidget* HitWidget;

	UPROPERTY()
	UUserWidget* EndGameWidget;

	UPROPERTY()
	UUserWidget* MiddleScene;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* IntroSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* EndingSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Phase1Sound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* Phase2Sound;

	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* AudioComp;
	

	bool bFirstPhase = true;
	bool bEndScreenStart = false;
};
