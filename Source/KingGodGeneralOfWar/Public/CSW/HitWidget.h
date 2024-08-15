// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UHitWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* HitAnim;

public:
	void PlayHitAnimation();
	
};
