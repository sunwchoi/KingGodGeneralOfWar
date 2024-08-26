// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_GodOfWar_GameModeBase.h"
#include "Components/WidgetComponent.h"
#include "BDthorFadeOut.h"

ASG_GodOfWar_GameModeBase::ASG_GodOfWar_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;

	

}

void ASG_GodOfWar_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	BDThorFadeOutUI = Cast<UBDthorFadeOut>(CreateWidget(GetWorld(), BDThorFadeOutFactory));

	if (GEngine) {
		GEngine->bEnableOnScreenDebugMessages = false; //디버그 메세지 안보이게 하기
	}
}

void ASG_GodOfWar_GameModeBase::ThorFadeOut()
{
	BDThorFadeOutUI->AddToViewport(0); //UI �ִϸ��̼� ���̱�
}
