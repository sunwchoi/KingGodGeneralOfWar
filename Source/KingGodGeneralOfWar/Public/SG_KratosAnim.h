// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kratos.h"
#include "SG_KratosAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDodgeEndCheckDelegate);
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

	void PlayAttackMontage();
	void PlayDodgeMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnDodgeEndCheckDelegate OnDodgeEndCheck;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerState();

	UPROPERTY(BlueprintReadOnly)
	EPlayerState PlayerState;
	UPROPERTY(BlueprintReadOnly)
	float Direction;
	UPROPERTY(BlueprintReadOnly)
	float Speed;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ATTACK, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage ;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck() ;

	FName GetAttackMontageSection(int32 Section);
};
