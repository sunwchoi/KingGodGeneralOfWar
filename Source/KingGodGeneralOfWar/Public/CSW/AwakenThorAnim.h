// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AwakenThorAnim.generated.h"

enum class EAwakenThorState : uint8;
/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UAwakenThorAnim : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM, meta=(AllowPrivateAccess))
	EAwakenThorState AnimState;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	class AAwakenThor* Owner;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	class UAwakenThorFSM* Fsm;

	UPROPERTY(EditDefaultsOnly)
	bool IsPlay;
public:
	UAwakenThorAnim();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetState(EAwakenThorState AnimState);
	EAwakenThorState GetState() const;

	UFUNCTION()
	void AnimNotify_AwakenThorTeleport_Play();
	
	UFUNCTION()
	void AnimNotify_AwakenThorTeleport_Throw();

	UFUNCTION()
	void AnimNotify_AwakenThorTeleport_Catch();

	UFUNCTION()
	void AnimNotify_ClapAttackStart();

	UFUNCTION()
	void AnimNotify_KickAttackStart();

	UFUNCTION()
	void AnimNotify_PoundAttackReady();

	UFUNCTION()
	void AnimNotify_PoundAttackStart();
	
	UFUNCTION()
	void AnimNotify_PoundAttackEnd();

	UFUNCTION()
	void AnimNotify_JumpAttackStart();

	UFUNCTION()
	void AnimNotify_JumpAttackReady();

	UFUNCTION()
	void AnimNotify_JumpAttackJump();

	UFUNCTION()
	void AnimNotify_JumpAttackFall();
	
	UFUNCTION()
	void AnimNotify_JumpAttackEnd();

	UFUNCTION()
	void AnimNotify_End();

	UFUNCTION()
	void AnimNotify_OffTrail();

	void PlayHitMontage(const FString& Section);

	void PlayDieMontage();

	void PlayClapAttackMontage();
	void PlayKickAttackMontage();
	void PlayPoundAttackMontage();
	void PlayTeleportMontage();
	void PlayJumpAttackMontage();
	void PlayKnockBackMontage();

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DieMontage;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> HitMontageArr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* KnockBackMontage;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ClapAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* KickAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* PoundAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TeleportMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* JumpAttackMontage;
	
	bool GetIsPlay() const;
	void SetIsPlay(bool Value);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float Vertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float Horizontal;
};
