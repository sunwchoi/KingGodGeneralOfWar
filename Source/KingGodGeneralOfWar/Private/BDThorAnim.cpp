// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorAnim.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "BDThorMjolnir.h"
#include "Kismet/GameplayStatics.h"

void UBDThorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*if (bdThor == nullptr || bdThor->IsValidLowLevel()) {
		auto ownerPawn = TryGetPawnOwner();
		bdThor = Cast<ABDThor>(ownerPawn);

	}

	if (bdThorFSM == nullptr || bdThorFSM->IsValidLowLevel()) {
		bdThorFSM = Cast<UBDThorFSM>(bdThor->fsm);
	}*/

	if (!bdThor) {
		auto ownerPawn = TryGetPawnOwner();
		//auto* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ABDThor::StaticClass());
		bdThor = Cast<ABDThor>(ownerPawn);
	}

	if (bdThor && !bdThorFSM) {
		bdThorFSM = Cast<UBDThorFSM>(bdThor->fsm);
	}

	if (!Mjolnir) {
		auto* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ABDThorMjolnir::StaticClass());
		Mjolnir = Cast<ABDThorMjolnir>(actor);
	}
}


void UBDThorAnim::playBDHammerThrow()
{
	if (!Montage_IsPlaying(BDHammerThrowMontage)) {
		Montage_Play(BDHammerThrowMontage);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Throw Animation"));
}

void UBDThorAnim::playBDHammerWind()
{
	if (!Montage_IsPlaying(BDHammerWindMontage)) {
		Montage_Play(BDHammerWindMontage);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Wind Animation"));
}

void UBDThorAnim::playBDBackDodge()
{
	if (!Montage_IsPlaying(BDThorBackDodgeMontage)) {
		Montage_Play(BDThorBackDodgeMontage); //뒤로 회피
	}
}

void UBDThorAnim::playBDRightDodge()
{
	if (!Montage_IsPlaying(BDThorRightDodgeMontage)) {
		Montage_Play(BDThorRightDodgeMontage); //오른쪽으로 회피
	}
}

void UBDThorAnim::playBDDamage()
{
	if (!Montage_IsPlaying(BDThorDamageMontage)) {
		Montage_Play(BDThorDamageMontage); //데미지가 발생될 때 생김
	}
}

void UBDThorAnim::playBDAttackRandomState()
{
	if (!Montage_IsPlaying(BDThorAttackRandomMontage)) {
		Montage_Play(BDThorAttackRandomMontage); //공격 랜덤 선택 상태가 발생할 때
	}
}

void UBDThorAnim::playBDHammerThreeSwing()
{
	if (!Montage_IsPlaying(BDHammerThreeSwingMontage)) {
		Montage_Play(BDHammerThreeSwingMontage); //공격 랜덤 선택 상태가 발생할 때
	}
}

void UBDThorAnim::AnimNotify_AnimEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("EndAnim"));
	if (bdThorFSM) {
		bdThorFSM->BDEndState(); // 애니메이션이 끝날 시 제어
		//UE_LOG(LogTemp, Warning, TEXT("Succed"));
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("Faild"));
	}
}

//BDHammerThrow 노티파이
void UBDThorAnim::AnimNotify_ThrowTiming()
{
	bdThor->BDHammerThrowHit();
}

void UBDThorAnim::AnimNotify_SlashWind()
{
	//슬래쉬 날리기
	bdThor->BDHammerWindSlash();
}

void UBDThorAnim::AnimNotify_BackMjolnir()
{
	Mjolnir->BackMjolnir(); //묠니르가 돌아오도록 호출
	UE_LOG(LogTemp, Warning, TEXT("Notify Back1"));
}
