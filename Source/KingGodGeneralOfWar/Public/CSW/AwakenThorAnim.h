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
	
	UFUNCTION(BlueprintImplementableEvent, Category=FSMEvent)
	void PlayPoundAttackAnim(FName SectionName);

	void SetState(EAwakenThorState AnimState);

	UFUNCTION()
	void AnimNotify_AwakenThorTeleport_Play();

	UFUNCTION()
	void AnimNotify_AwakenThorTeleport_End();

	UFUNCTION()
	void AnimNotify_AwakenThorTeleport_Throw();

	UFUNCTION()
	void AnimNotify_AwakenThorTeleport_Catch();

	bool GetIsPlay() const;
	void SetIsPlay(bool Value);
};