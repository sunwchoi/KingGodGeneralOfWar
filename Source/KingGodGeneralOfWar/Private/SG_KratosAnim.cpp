// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"
#include "Axe.h"
#include "FlyingAxe.h"
USG_KratosAnim::USG_KratosAnim()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_WeakAttack.AM_Kratos_WeakAttack'")
	);
	if (TempAttackMontage.Succeeded())	WeakAttackMontage = TempAttackMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempStrongAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_MeleeAttack.AM_Kratos_MeleeAttack'")
	);
	if (TempStrongAttackMontage.Succeeded())	StrongAttackMontage = TempStrongAttackMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempDodgeMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Dodge.AM_Kratos_Dodge'")
	);
	if (TempDodgeMontage.Succeeded())	DodgeMontage = TempDodgeMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempRollMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Roll.AM_Kratos_Roll'")
	);
	if (TempRollMontage.Succeeded())	RollMontage = TempRollMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempGuardMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Guard.AM_Kratos_Guard'")
	);
	if (TempGuardMontage.Succeeded())	GuardMontage = TempGuardMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempAxeThrowMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_AxeThrow.AM_AxeThrow'")
	);
	if (TempAxeThrowMontage.Succeeded())	AxeThrowMontage = TempAxeThrowMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempAxeWithdrawMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_AxeWithdraw.AM_Kratos_AxeWithdraw'")
	);
	if (TempAxeWithdrawMontage.Succeeded())	AxeWithdrawMontage = TempAxeWithdrawMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempDashAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_DashAttack.AM_Kratos_DashAttack'")
	);
	if (TempDashAttackMontage.Succeeded())	DashAttackMontage = TempDashAttackMontage.Object;
	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempRuneBaseMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_RuneBase.AM_Kratos_RuneBase'")
	);
	if (TempRuneBaseMontage.Succeeded())	RuneBaseMontage = TempRuneBaseMontage.Object;
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
		Speed = FVector::DotProduct(forwardVector, velocity);
		Direction = FVector::DotProduct(Player->GetActorRightVector(), velocity);
	}
}
void USG_KratosAnim::PlayWeakAttackMontage()
{
	Montage_Play(WeakAttackMontage, 1.0f);

}
void USG_KratosAnim::PlayStrongAttackMontage()
{
	Montage_Play(StrongAttackMontage, 1.0f);
}

void USG_KratosAnim::PlayDodgeMontage()
{
	Montage_Play(DodgeMontage, 1.0f);
}

void USG_KratosAnim::PlayRollMontage()
{
	Montage_Play(RollMontage, 1.0f);
}

void USG_KratosAnim::PlayGuardMontage()
{
	Montage_Play(GuardMontage, 1.0f);
}

void USG_KratosAnim::PlayAxeThrowMontage()
{
	Montage_Play(AxeThrowMontage, 1.0f);
	AKratos* Player = Cast<AKratos>(GetOwningActor());
	TargetRotation = Player->GetControlRotation();
}

void USG_KratosAnim::PlayAxeWithdrawMontage()
{
	FlyingAxe->WithdrawAxe();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			check(AxeWithdrawMontage);
			if (AxeWithdrawMontage)
				Montage_Play(AxeWithdrawMontage);
			
		}, 1.15f, false);

}

void USG_KratosAnim::JumpToStrongAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSection(NewSection), StrongAttackMontage);
}

void USG_KratosAnim::JumpToWeakAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSection(NewSection), WeakAttackMontage);
}

void USG_KratosAnim::JumpToDodgeMontageSection(FString SectionName)
{
	Montage_JumpToSection(FName(*SectionName), DodgeMontage);
}

void USG_KratosAnim::JumpToRollMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetRollMontageSection(NewSection), RollMontage);
}

void USG_KratosAnim::JumpToGuardMontageSection(FString SectionName)
{
	Montage_JumpToSection(FName(*FString(SectionName)));
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

void USG_KratosAnim::AnimNotify_NextWeakAttackCheck()
{
	OnNextWeakAttackCheck.Broadcast();
}

void USG_KratosAnim::AnimNotify_MovableCheck()
{
	OnMovableCheck.Broadcast();
}

void USG_KratosAnim::AnimNotify_GuardLoopStartCheck()
{
	AKratos* Player = Cast<AKratos>(GetOwningActor());
	if (Player)
	{
		if (Player->State == EPlayerState::GuardStart)
			Player->State = EPlayerState::Guard;
	}
}

void USG_KratosAnim::AnimNotify_HideAxe()
{
	AKratos* Player = Cast<AKratos>(GetOwningActor());
	if (Player)
	{
		auto* Axe = Player->CurrentWeapon;
		if (Axe)
		{
			Axe->MeshComp->SetVisibility(false, true);
			FlyingAxe = GetWorld()->SpawnActor<AFlyingAxe>(FlyingAxeFactory, Axe->GetActorLocation(), TargetRotation);
		}
	}
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


FName USG_KratosAnim::GetGuardMontageSection(int32 Section)
{
	return FName();
}
