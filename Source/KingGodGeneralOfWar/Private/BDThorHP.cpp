// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorHP.h"
#include "Components/ProgressBar.h"

void UBDThorHP::NativeConstruct()
{
	Super::NativeConstruct();

	//BDHPBar->SetPercent(1);

}

void UBDThorHP::SetHP(float CurHp, float MaxHp)
{
	BDHPBar->SetPercent(CurHp / MaxHp);
}
