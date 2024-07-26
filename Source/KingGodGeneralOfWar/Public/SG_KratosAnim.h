// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kratos.h"
#include "SG_KratosAnim.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API USG_KratosAnim : public UAnimInstance
{
	GENERATED_BODY()
	
private:

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UFUNCTION()
	EPlayerState GetPlayerState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	EPlayerState PlayerState;

};
