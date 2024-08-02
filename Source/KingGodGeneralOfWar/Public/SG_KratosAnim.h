// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kratos.h"
#include "SG_KratosAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate);
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

	void JumpToStrongAttackMontageSection(int32 NewSection);
	void JumpToWeakAttackMontageSection(int32 NewSection);
	void JumpToDodgeMontageSection(int32 NewSection);
	void JumpToRollMontageSection(int32 NewSection);
	void JumpToGuardMontageSection(FString SectionName);

	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnAttackEndCheckDelegate OnAttackEndCheck;
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
private:

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_AttackEndCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck() ;

	UFUNCTION()
	void AnimNotify_MovableCheck();

	UFUNCTION()
	void AnimNotify_GuardLoopStartCheck();
	
	UFUNCTION()
	void AnimNotify_HideAxe();

	FName GetWeakAttackMontageSection(int32 Section);
	FName GetStrongAttackMontageSection(int32 Section);
	FName GetRollMontageSection(int32 Section);
	FName GetDodgeMontageSection(int32 Section);
	FName GetGuardMontageSection(int32 Section);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFlyingAxe> FlyingAxeFactory;

	class AFlyingAxe* FlyingAxe;

	FRotator TargetRotation;
};
