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

	void StartFirstPhase();
	void StartSecondPhase();
	void GoToNextPhase();
	void EndWithFail();
	void EndWithSucceed();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UOutGameWidget> WBP_GameStart;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameWidget> WBP_InGame;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOutGameWidget> WBP_GameFail;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WBP_GameSucceed;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AAwakenThor> BP_AwakenThor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABDThor> BP_BDThor;
	
	UPROPERTY()
	UOutGameWidget* OutGameWidget;

	UPROPERTY()
	class UInGameWidget* InGameWidget;

	UPROPERTY()
	UUserWidget* EndGameWidget;

	bool bFirstPhase = true;
	bool bEndScreenStart = false;
};
