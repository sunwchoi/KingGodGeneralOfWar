// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"
#include "Axe.h"
#include "FlyingAxe.h"
#include "Kismet/GameplayStatics.h"
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


	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempRuneAttackMontage(
		TEXT("/ Script / Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_RuneAttack.AM_Kratos_RuneAttack'")
	);
	if (TempRuneAttackMontage.Succeeded())	RuneAttackMontage = TempRuneAttackMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempHitMontage(
		TEXT("Script / Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Hit.AM_Kratos_Hit'")
	);
	if (TempHitMontage.Succeeded())	HitMontage = TempHitMontage.Object;
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
	if (!Montage_IsPlaying(WeakAttackMontage))
		Montage_Play(WeakAttackMontage, 1.0f);

}
void USG_KratosAnim::PlayStrongAttackMontage()
{
	if (!Montage_IsPlaying(StrongAttackMontage))
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

void USG_KratosAnim::PlayRuneBaseMontage()
{
	if (!Montage_IsPlaying(RuneBaseMontage))
		Montage_Play(RuneBaseMontage);
}

void USG_KratosAnim::PlayHitMontage()
{
	if (!Montage_IsPlaying(HitMontage))
		Montage_Play(HitMontage);
}

void USG_KratosAnim::PlayDashAttackMontage()
{
	Montage_Play(DashAttackMontage);
}

void USG_KratosAnim::PlayRuneAttackMontage()
{
	if (!Montage_IsPlaying(RuneAttackMontage))
		Montage_Play(RuneAttackMontage);
}

void USG_KratosAnim::JumpToAttackMontageSection(int32 NewSection)
{
	if (Montage_IsPlaying(StrongAttackMontage))
	{
		Montage_JumpToSection(GetAttackMontageSection(NewSection), StrongAttackMontage);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("StrongAttackMontage"));
	}
	else if (Montage_IsPlaying(WeakAttackMontage))
	{
		Montage_JumpToSection(GetAttackMontageSection(NewSection), WeakAttackMontage);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("StrongAttackMontage"));
	}
	else if (Montage_IsPlaying(RuneAttackMontage))
	{
		Montage_JumpToSection(GetAttackMontageSection(NewSection), RuneAttackMontage);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("StrongAttackMontage"));
	}
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
	Montage_JumpToSection(FName(*SectionName), GuardMontage);
}

void USG_KratosAnim::JumpToHitMontageSection(FString SectionName)
{
	Montage_JumpToSection(FName(*SectionName), HitMontage);
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

void USG_KratosAnim::AnimNotify_TimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.06f);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 0.02f, false);
}

void USG_KratosAnim::AnimNotify_FieldSpawn()
{
	
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
