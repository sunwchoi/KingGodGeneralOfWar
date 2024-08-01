// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorAnim.h"
#include "BDThor.h"
#include "BDThorFSM.h"

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
		bdThor = Cast<ABDThor>(ownerPawn);
	}

	if (bdThor && !bdThorFSM) {
		bdThorFSM = Cast<UBDThorFSM>(bdThor->fsm);
	}

}


void UBDThorAnim::playBDHammerThrow()
{
	if (!Montage_IsPlaying(BDHammerThrowMontage)) {
		Montage_Play(BDHammerThrowMontage);
	}
	UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Throw Animation"));
}

void UBDThorAnim::playBDHammerWind()
{
	if (!Montage_IsPlaying(BDHammerWindMontage)) {
		Montage_Play(BDHammerWindMontage);
	}
	UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Wind Animation"));
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
