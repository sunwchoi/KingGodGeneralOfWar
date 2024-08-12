// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/OutGameWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UOutGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartHover->SetVisibility(ESlateVisibility::Hidden);
	ExitHover->SetVisibility(ESlateVisibility::Hidden);
	StartButton->OnClicked.AddDynamic(this, &UOutGameWidget::OnStartButtonClick);
	StartButton->OnHovered.AddDynamic(this, &UOutGameWidget::OnStartButtonHovered);
	StartButton->OnUnhovered.AddDynamic(this, &UOutGameWidget::OnStartButtonUnhovered);

	ExitButton->OnClicked.AddDynamic(this, &UOutGameWidget::OnExitButtonClick);
	ExitButton->OnHovered.AddDynamic(this, &UOutGameWidget::OnExitButtonHovered);
	ExitButton->OnUnhovered.AddDynamic(this, &UOutGameWidget::OnExitButtonUnhovered);
}

void UOutGameWidget::OnStartButtonClick()
{
	RemoveFromParent();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}
	PlayerController->SetPause(false);
	
	Destruct();
}

void UOutGameWidget::OnStartButtonHovered()
{
	StartHover->SetVisibility(ESlateVisibility::Visible);
}

void UOutGameWidget::OnStartButtonUnhovered()
{
	StartHover->SetVisibility(ESlateVisibility::Hidden);
}

void UOutGameWidget::OnExitButtonClick()
{
}

void UOutGameWidget::OnExitButtonHovered()
{
	ExitHover->SetVisibility(ESlateVisibility::Visible);
}

void UOutGameWidget::OnExitButtonUnhovered()
{
	ExitHover->SetVisibility(ESlateVisibility::Hidden);
}

void UOutGameWidget::SetSecondPhase(bool Value)
{
	bSecondPhase = Value;
}
