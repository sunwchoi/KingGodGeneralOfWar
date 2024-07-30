// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"


USG_KratosAnim::USG_KratosAnim()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> ATTACK_MONTAGE(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_MeleeAttack.AM_Kratos_MeleeAttack'")
	);
	static ConstructorHelpers::FObjectFinder <UAnimMontage> DODGE_MONTAGE(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Dodge.AM_Kratos_Dodge'")
	);
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;
	if (DODGE_MONTAGE.Succeeded())
		DodgeMontage = DODGE_MONTAGE.Object;
}

void USG_KratosAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	UpdatePlayerState();

}

void USG_KratosAnim::UpdatePlayerState()
{
	AKratos* Player = Cast<AKratos>(GetOwningActor());
	if (Player)
	{
		FVector velocity = Player->GetVelocity();
		FVector forwardVector = Player->GetActorForwardVector();

		PlayerState = Player->State;
		//if (PlayerState == EPlayerState::Idle)
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, Player->GetPlayerStateString());
		Speed = FVector::DotProduct(forwardVector, velocity);
		Direction = FVector::DotProduct(Player->GetActorRightVector(), velocity);
	}
}
void USG_KratosAnim::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void USG_KratosAnim::PlayDodgeMontage()
{
	Montage_Play(DodgeMontage, 1.0f);
}

void USG_KratosAnim::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSection(NewSection), AttackMontage);
}


void USG_KratosAnim::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void USG_KratosAnim::AnimNotify_AttackEndCheck()
{
	OnAttackEndCheck.Broadcast();
}

void USG_KratosAnim::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName USG_KratosAnim::GetAttackMontageSection(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
