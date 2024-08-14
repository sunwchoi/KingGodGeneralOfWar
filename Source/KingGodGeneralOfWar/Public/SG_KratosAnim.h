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

	void PlayDodgeMontage();
	void PlayRollMontage();
	void PlayGuardMontage();
	void PlayRuneBaseMontage();
	void PlayHitMontage();
	void PlayWeakAttackMontage();
	void PlayStrongAttackMontage();
	bool PlayAxeThrowMontage();
	void PlayAxeWithdrawMontage();
	void PlayDashAttackMontage();
	void PlayRuneAttackMontage();
	void PlayParryAttackMontage();

	void JumpToAttackMontageSection(int32 NewSection);
	void JumpToDodgeMontageSection(FString SectionName);
	void JumpToRollMontageSection(int32 NewSection);
	void JumpToGuardMontageSection(FString SectionName);
	void JumpToHitMontageSection(FString SectionName);

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

	UPROPERTY()
	class UAnimMontage* WeakAttackMontage ;

	UPROPERTY()
	class UAnimMontage* StrongAttackMontage ;

	UPROPERTY()
	class UAnimMontage* DodgeMontage;

	UPROPERTY()
	class UAnimMontage* RollMontage;

	UPROPERTY()
	class UAnimMontage* GuardMontage;

	UPROPERTY()
	class UAnimMontage* AxeThrowMontage;

	UPROPERTY()
	class UAnimMontage* AxeWithdrawMontage;

	UPROPERTY()
	class UAnimMontage* DashAttackMontage;

	UPROPERTY()
	class UAnimMontage* RuneBaseMontage;

	UPROPERTY()
	class UAnimMontage* RuneAttackMontage;

	UPROPERTY()
	class UAnimMontage* ParryAttackMontage ;

	UPROPERTY()
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ARuneAttackField> RuneAttackFieldFactory;

	UPROPERTY()
	class AKratos* Me;
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

	UFUNCTION()
	void AnimNotify_TimeDilation();

	UFUNCTION()
	void AnimNotify_FieldSpawn();

	UFUNCTION()
	void AnimNotify_RuneAttackGroundShake();

	UFUNCTION()
	void AnimNotify_RuneAttackCameraSet();

	UFUNCTION()
	void AnimNotify_GetUPCameraSet();

	UFUNCTION()
	void AnimNotify_RuneReady();

	UFUNCTION()
	void AnimNotify_RuneAttackEnd();

	UFUNCTION()
	void AnimNotify_SpawnEarthCrack();

	UFUNCTION()
	void AnimNotify_AttackComboEnd();

	UFUNCTION()
	void AnimNotify_InitAttackType();

	UFUNCTION()
	void AnimNotify_AttackProgress();

	UFUNCTION()
	void AnimNotify_GuardDisappear();

	UFUNCTION()
	void AnimNotify_LaunchCharacterInStrongAttack();

	UFUNCTION()
	void AnimNotify_EndWithFail();

	UFUNCTION()
	void AnimNotify_JumpCharacterInStrongAttack();
	FName GetAttackMontageSection(int32 Section);
	FName GetRollMontageSection(int32 Section);
	FName GetGuardMontageSection(int32 Section);

	FRotator TargetRotation;
};
