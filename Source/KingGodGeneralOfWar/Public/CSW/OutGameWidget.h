// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UOutGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartButtonClick();

	UFUNCTION()
	void OnStartButtonHovered();

	UFUNCTION()
	void OnStartButtonUnhovered();

	UFUNCTION()
	void OnExitButtonClick();

	UFUNCTION()
	void OnExitButtonHovered();

	UFUNCTION()
	void OnExitButtonUnhovered();

	void SetSecondPhase(bool Value);
	
	
private:
	UPROPERTY(meta=(BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta=(BindWidget))
	class UImage* StartHover;

	UPROPERTY(meta=(BindWidget))
	class UImage* ExitHover;

	UPROPERTY(meta=(BindWidget))
	bool bSecondPhase;

};
