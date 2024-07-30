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

	static ConstructorHelpers::FObjectFinder <UAnimMontage> ROLL_MONTAGE(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Roll.AM_Kratos_Roll'")
	);
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;
	if (DODGE_MONTAGE.Succeeded())
		DodgeMontage = DODGE_MONTAGE.Object;
	if (ROLL_MONTAGE.Succeeded())
		RollMontage = ROLL_MONTAGE.Object;
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

void USG_KratosAnim::PlayRollMontage()
{
	Montage_Play(RollMontage, 1.0f);
}

void USG_KratosAnim::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSection(NewSection), AttackMontage);
}

void USG_KratosAnim::JumpToDodgeMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetDodgeMontageSection(NewSection), DodgeMontage);
}

void USG_KratosAnim::JumpToRollMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetRollMontageSection(NewSection), RollMontage);
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

void USG_KratosAnim::AnimNotify_MovableCheck()
{
	OnMovableCheck.Broadcast();
}

FName USG_KratosAnim::GetAttackMontageSection(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

FName USG_KratosAnim::GetRollMontageSection(int32 Section)
{
	TArray<FString> RollSectionName = { TEXT("Left") , TEXT("Right") , TEXT("Forward") ,TEXT("Back") };
	FName SectionName(*FString::Printf(TEXT("Roll_%s"), *RollSectionName[Section]));
	return SectionName;
}

FName USG_KratosAnim::GetDodgeMontageSection(int32 Section)
{
	TArray<FString> RollSectionName = { TEXT("Left") , TEXT("Right") , TEXT("Forward") ,TEXT("Back") };
	FName SectionName(*FString::Printf(TEXT("Dodge_%s"), *RollSectionName[Section]));
	return SectionName;
}
