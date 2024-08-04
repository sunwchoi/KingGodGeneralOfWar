// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/HpBar.h"

#include "Components/ProgressBar.h"

void UHpBar::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar->SetPercent(1);
}

void UHpBar::SetHp(float Hp, float MaxHp)
{
	HpBar->SetPercent(Hp / MaxHp);
}


