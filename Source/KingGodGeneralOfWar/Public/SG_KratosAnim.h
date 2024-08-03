// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kratos.h"
#include "SG_KratosAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextWeakAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWeakAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWeakAttackEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMovableChekcDelegate);
/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API USG_KratosAnim : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	
public:
	USG_KratosAnim();
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void PlayWeakAttackMontage();
	void PlayStrongAttackMontage();
	void PlayDodgeMontage();
	void PlayRollMontage();
	void PlayGuardMontage();
	void PlayAxeThrowMontage();
	void PlayAxeWithdrawMontage();
	void PlayRuneBaseMontage();
	void PlayDashAttackMontage();
	void PlayRuneAttackMontage();

	void JumpToStrongAttackMontageSection(int32 NewSection);
	void JumpToWeakAttackMontageSection(int32 NewSection);
	void JumpToDodgeMontageSection(FString SectionName);
	void JumpToRollMontageSection(int32 NewSection);
	void JumpToGuardMontageSection(FString SectionName);

	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnAttackEndCheckDelegate OnAttackEndCheck;
	FOnNextWeakAttackCheckDelegate OnNextWeakAttackCheck;
	FOnMovableChekcDelegate OnMovableCheck;
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerState();

	UPROPERTY(BlueprintReadOnly)
	EPlayerState PlayerState;
	UPROPERTY(BlueprintReadOnly)
	float Direction;
	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* WeakAttackMontage ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* StrongAttackMontage ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* GuardMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AxeThrowMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AxeWithdrawMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DashAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RuneBaseMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RuneAttackMontage;
private:

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_AttackEndCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck() ;

	UFUNCTION()
	void AnimNotify_NextWeakAttackCheck();

	UFUNCTION()
	void AnimNotify_MovableCheck();

	UFUNCTION()
	void AnimNotify_GuardLoopStartCheck();
	
	UFUNCTION()
	void AnimNotify_HideAxe();

	FName GetAttackMontageSection(int32 Section);
	FName GetRollMontageSection(int32 Section);
	FName GetGuardMontageSection(int32 Section);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFlyingAxe> FlyingAxeFactory;

	class AFlyingAxe* FlyingAxe;

	FRotator TargetRotation;
};
