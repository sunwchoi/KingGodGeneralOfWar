// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/AttackZone.h"
#include "PoundThunderAttackZone.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API APoundThunderAttackZone : public AAttackZone
{
	GENERATED_BODY()

public:
	APoundThunderAttackZone();

	UFUNCTION(BlueprintCallable)
	void Active();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp0;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp1;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp2;
};
