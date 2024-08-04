// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPUI.h"
#include "Components/ProgressBar.h"
void UPlayerHPUI::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar->SetPercent(1);
}

void UPlayerHPUI::SetHP(float HP, float MaxHP)
{
	HpBar->SetPercent(HP / MaxHP);
}
