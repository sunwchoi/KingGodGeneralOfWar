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

void UAwakenThorAnim::AnimNotify_KickAttackStart()
{
	Fsm->StartKickAttack();
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

void UAwakenThorAnim::AnimNotify_End()
{
	Fsm->OnEnd();
}

void UAwakenThorAnim::AnimNotify_OffTrail()
{
}

void UAwakenThorAnim::PlayHitMontage(const FString& Section)
{
	int32 rand = FMath::RandRange(0, HitMontageArr.Num() - 1);
	Montage_Play(HitMontageArr[rand]);
	Montage_JumpToSection(FName(*Section), HitMontageArr[rand]);
}

void UAwakenThorAnim::PlayDieMontage()
{
	if (!Montage_IsPlaying(DieMontage))
		Montage_Play(DieMontage);
}

void UAwakenThorAnim::PlayClapAttackMontage()
{
	if(!Montage_IsPlaying(ClapAttackMontage))
		Montage_Play(ClapAttackMontage);
}

void UAwakenThorAnim::PlayKickAttackMontage()
{
	if (!Montage_IsPlaying(KickAttackMontage))
		Montage_Play(KickAttackMontage);
}

void UAwakenThorAnim::PlayPoundAttackMontage()
{
	if (!Montage_IsPlaying(PoundAttackMontage))
		Montage_Play(PoundAttackMontage);
}

void UAwakenThorAnim::PlayTeleportMontage()
{
	if (!Montage_IsPlaying(TeleportMontage))
		Montage_Play(TeleportMontage);
}

void UAwakenThorAnim::PlayJumpAttackMontage()
{
	if (!Montage_IsPlaying(JumpAttackMontage))
		Montage_Play(JumpAttackMontage);
}

void UAwakenThorAnim::PlayKnockBackMontage()
{
	if (!Montage_IsPlaying(KnockBackMontage))
		Montage_Play(KnockBackMontage);
}


bool UAwakenThorAnim::GetIsPlay() const
{
	return IsPlay;
}

void UAwakenThorAnim::SetIsPlay(bool Value)
{
	IsPlay = Value;
}
