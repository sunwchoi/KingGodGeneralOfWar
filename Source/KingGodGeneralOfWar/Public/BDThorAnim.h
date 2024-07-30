// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BDThorFSM.h"
#include "BDThorAnim.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UBDThorAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BDThorFSM)
	class UBDThorFSM* bdThorFSM; //FSM

	UPROPERTY(EditDefaultsOnly, Category = BDThor)
	class ABDThor* bdThor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState animState; //FSM 상태


	//BDHammerThrow 몽타주
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDHammerThrowMontage;

	//BDHammerWind 몽타주
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDHammerWindMontage;


	//몽타주 실행
	UFUNCTION()
	void playBDHammerThrow();
	UFUNCTION()
	void playBDHammerWind();


	UFUNCTION()
	void AnimNotify_AnimEnd(); //애니메이션이 끝났을 때 상태를 바꿀 노티파이

	//BDHammerThrow 몽타주
	UFUNCTION()
	void AnimNotify_ThrowTiming();


	UFUNCTION()
	void AnimNotify_SlashWind();
	
};
