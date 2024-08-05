// Fill out your copyright notice in the Description page of Project Settings.


#include "BDthorFadeOut.h"
#include "Kismet/GameplayStatics.h"

void UBDthorFadeOut::NativeConstruct()
{
	Super::NativeConstruct();

	FadeOutAnimStart();

}

void UBDthorFadeOut::FadeOutAnimStart()
{
	PlayAnimationForward(StartAnim); //페이드 아웃 애니메이션 시작

	//UGameplayStatics::OpenLevel(GetWorld(), FName("CSWMap")); //레벨 변경

	// 2초 후에 OnFadeOutCompleted 함수를 호출하는 타이머를 설정합니다.
	GetWorld()->GetTimerManager().SetTimer(handle, this, &UBDthorFadeOut::OnFadeOutCompleted, 3.0f, false);

}

void UBDthorFadeOut::OnFadeOutCompleted()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("CSWMap")); // 레벨 변경
}

