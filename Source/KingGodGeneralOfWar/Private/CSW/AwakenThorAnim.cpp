// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThorAnim.h"
#include "CSW/AwakenThorFSM.h"
#include "CSW/AwakenThor.h"

UAwakenThorAnim::UAwakenThorAnim()
{
}

void UAwakenThorAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AAwakenThor>(TryGetPawnOwner());
	Fsm = Owner->getFSM();
}

void UAwakenThorAnim::SetState(EAwakenThorState State)
{
	AnimState = State;
}

void UAwakenThorAnim::AnimNotify_AwakenThorTeleport_Play()
{
	IsPlay = true;
}

void UAwakenThorAnim::AnimNotify_AwakenThorTeleport_End()
{
	IsPlay = false;
	AnimState = EAwakenThorState::Idle;
	Fsm->SetState(EAwakenThorState::Idle);
}

void UAwakenThorAnim::AnimNotify_AwakenThorTeleport_Throw()
{
	Fsm->ThrowForTeleport();
}

void UAwakenThorAnim::AnimNotify_AwakenThorTeleport_Catch()
{
	Fsm->Teleport();
}

bool UAwakenThorAnim::GetIsPlay() const
{
	return IsPlay;
}

void UAwakenThorAnim::SetIsPlay(bool Value)
{
	IsPlay = Value;
}
