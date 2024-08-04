// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDThorHP.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UBDThorHP : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* BDHPBar;

	void SetHP(float CurHp, float MaxHp);
	
};
