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
public:
	UFUNCTION(BlueprintImplementableEvent, Category=FSMEvent)
	void PlayPoundAttackAnim(FName SectionName);

	void SetState(EAwakenThorState AnimState);
};
