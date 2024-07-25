// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AwakenThorFSM.generated.h"

UENUM()
enum class EAwakenThorState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGGODGENERALOFWAR_API UAwakenThorFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAwakenThorFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	UFUNCTION()
	void SetState(EAwakenThorState NewState);

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM, meta=(AllowPrivateAccess))
	EAwakenThorState State = EAwakenThorState::Idle;

	UPROPERTY(VisibleAnywhere, Category=FSM)
	ACharacter* Target;
	
	UPROPERTY()
	class AAwakenThor* Me;
	
	float IdleDelayTime = 2.f;
	float CurrentTime;
	
	float TeleportDist = 1000;

	FTimerHandle MoveTimerHandle;
};
