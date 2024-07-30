// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AwakenThorFSM.generated.h"

UENUM(BlueprintType)
enum class EAwakenThorState : uint8
{
	Idle,
	Move,
	PoundAttack,
	ClapAttack,
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



	UFUNCTION(BlueprintCallable)
	void SetState(EAwakenThorState NewState);

	void IdleState();
	void MoveState();
	void PoundAttackState();
	void ClapAttackState();
	void DamageState();
	void DieState();

	void ThrowForTeleport();
	void Teleport();
	void ReadyPoundAttack();
	void StartPoundAttack();
	void StartClapAttack();


private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=FSM, meta=(AllowPrivateAccess))
	EAwakenThorState State = EAwakenThorState::Idle;

	UPROPERTY(VisibleAnywhere, Category=FSM)
	ACharacter* Target;
	
	UPROPERTY()
	class AAwakenThor* Me;

	UPROPERTY()
	class UAwakenThorAnim* Anim;
	
	float IdleDelayTime = 5.f;
	float CurrentTime;
	
	float TeleportDist = 1000;

	FTimerHandle MoveTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bPlay;

	TArray<FVector> AttackZone;
};
