// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FPlayerHpDelegate, float);
DECLARE_DELEGATE_OneParam(FEnemyHpDelegate, float);

UCLASS()
class KINGGODGENERALOFWAR_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetPlayerHpBar(float Percent);

	UFUNCTION()
	void SetEnemyHpBar(float Percent);

	UFUNCTION()
	void SetThorName(bool Value);

private:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PlayerHpBar;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* EnemyHpBar;

	UPROPERTY(meta=(BindWidget))
	class UImage* ThorText;

	UPROPERTY(meta=(BindWidget))
	class UImage* AwakenThorText;
};
