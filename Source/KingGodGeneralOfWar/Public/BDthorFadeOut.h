// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDthorFadeOut.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UBDthorFadeOut : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation* StartAnim; //페이드 아웃 시작

	UFUNCTION()
	void FadeOutAnimStart();

	UFUNCTION()
	void OnFadeOutCompleted();
	
	FTimerHandle handle;
	
};
