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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	class UBDThorFSM* bdThorFSM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState animState;



	
};
