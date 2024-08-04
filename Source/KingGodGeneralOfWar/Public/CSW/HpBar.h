// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpBar.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UHpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HpBar;

	void SetHp(float Hp, float MaxHp);
};
