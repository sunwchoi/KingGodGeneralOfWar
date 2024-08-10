// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/InGameWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FPlayerHpDelegate PlayerHpDelegate;
	FEnemyHpDelegate EnemyHpDelegate;
	
	PlayerHpDelegate.BindUFunction(this, TEXT("SetPlayerHpBar"));
	EnemyHpDelegate.BindUFunction(this, TEXT("SetEnemyHpBar"));

	if (EnemyHpDelegate.IsBound())
		UE_LOG(LogTemp, Warning, TEXT("Bind!"));
}

void UInGameWidget::SetPlayerHpBar(float Percent)
{
	PlayerHpBar->SetPercent(Percent);
}

void UInGameWidget::SetEnemyHpBar(float Percent)
{
	EnemyHpBar->SetPercent(Percent);
}

void UInGameWidget::SetThorName(bool IsThor)
{
	ThorText->SetVisibility(ESlateVisibility::Hidden); //Thor
	AwakenThorText->SetVisibility(ESlateVisibility::Hidden);
	if (IsThor)
		ThorText->SetVisibility(ESlateVisibility::Visible); //Thor
	else
		AwakenThorText->SetVisibility(ESlateVisibility::Visible); //Awaken Thor
}
