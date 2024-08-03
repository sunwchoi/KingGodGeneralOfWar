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

void UAwakenThorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Owner == nullptr)
		return ;
	FVector v = Owner->GetVelocity();
	Vertical = FVector::DotProduct(Owner->GetActorForwardVector(), v);
	Horizontal = FVector::DotProduct(Owner->GetActorRightVector(), v);
}

void UAwakenThorAnim::SetState(EAwakenThorState State)
{
	AnimState = State;
}

EAwakenThorState UAwakenThorAnim::GetState() const
{
	return AnimState;
}

void UAwakenThorAnim::AnimNotify_AwakenThorTeleport_Play()
{
	Fsm->LookTeleportDirection();
}

void UAwakenThorAnim::AnimNotify_AwakenThorTeleport_End()
{
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

void UAwakenThorAnim::AnimNotify_ClapAttackStart()
{
	Fsm->StartClapAttack();
}

void UAwakenThorAnim::AnimNotify_ClapAttackEnd()
{
	AnimState = EAwakenThorState::Idle;
	Fsm->SetState(EAwakenThorState::Idle);
}

void UAwakenThorAnim::AnimNotify_PoundAttackReady()
{
	Fsm->ReadyPoundAttack();
}

void UAwakenThorAnim::AnimNotify_PoundAttackStart()
{
	Fsm->StartPoundAttack();
}

void UAwakenThorAnim::AnimNotify_PoundAttackEnd()
{
	AnimState = EAwakenThorState::Idle;
	Fsm->SetState(EAwakenThorState::Idle);
}

void UAwakenThorAnim::AnimNotify_KickStart()
{
	Fsm->StartKickAttack();
}

void UAwakenThorAnim::AnimNotify_KickEnd()
{
	AnimState = EAwakenThorState::Idle;
	Fsm->SetState(EAwakenThorState::Idle);
}

void UAwakenThorAnim::AnimNotify_HitEnd()
{
	AnimState = EAwakenThorState::Idle;
	Fsm->SetState(EAwakenThorState::Idle);
}

void UAwakenThorAnim::AnimNotify_JumpAttackReady()
{
	Fsm->ReadyJumpAttack();
}

void UAwakenThorAnim::AnimNotify_JumpAttackJump()
{
	Owner->Jump();
}

void UAwakenThorAnim::AnimNotify_JumpAttackFall()
{
	Fsm->StartFallAttack();
}

void UAwakenThorAnim::AnimNotify_JumpAttackStart()
{
	Fsm->StartJumpAttack();
}


void UAwakenThorAnim::AnimNotify_JumpAttackEnd()
{
	AnimState = EAwakenThorState::Idle;
	Fsm->SetState(EAwakenThorState::Idle);
}


bool UAwakenThorAnim::GetIsPlay() const
{
	return IsPlay;
}

void UAwakenThorAnim::SetIsPlay(bool Value)
{
	IsPlay = Value;
}
