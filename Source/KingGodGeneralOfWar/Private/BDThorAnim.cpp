// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorAnim.h"
#include "BDThor.h"
#include "BDThorFSM.h"

void UBDThorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner();
	bdThor = Cast<ABDThor>(ownerPawn);
}


void UBDThorAnim::playBDHammerThrow()
{
	Montage_Play(BDHammerThrowMontage);
}

void UBDThorAnim::AnimNotify_AnimEnd()
{
}

//BDHammerThrow 노티파이
void UBDThorAnim::AnimNotify_ThrowTiming()
{
	bdThor->BDHammerThrowHit();
}
