// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kratos.h"
#include "Components/ActorComponent.h"
#include "AwakenThorFSM.generated.h"

UENUM(BlueprintType)
enum class EAwakenThorState : uint8
{
	Idle,
	Move,
	Dash,
	LeftTeleport,
	RightTeleport,
	BackTeleport,
	Teleport,
	MeleeAttackChange,
	RangedAttackChange,
	PoundAttack,
	JumpAttack,
	ClapAttack,
	KickAttack,
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
	void DashState();
	void TeleportState();
	void MeleeAttackChangeState();
	void RangedAttackChangeState();
	void JumpAttackState();
	void PoundAttackState();
	void KickAttackState();
	void ClapAttackState();
	void DamageState();
	void DieState();

	void LookTeleportDirection();
	void ThrowForTeleport();
	void Teleport();
	void ReadyPoundAttack();
	void StartPoundAttack();
	void StartClapAttack();
	void StartKickAttack();
	void ReadyJumpAttack();
	void StartJumpAttack();
	void StartFallAttack();

	void SetDamage(float Damage = 1, EAttackDirectionType AtkDir = EAttackDirectionType::FORWARD);
	void SetJump(bool Value);


private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=FSM, meta=(AllowPrivateAccess))
	EAwakenThorState State = EAwakenThorState::Idle;

	UPROPERTY(VisibleAnywhere, Category=FSM)
	AKratos* Target;
	
	UPROPERTY()
	class AAwakenThor* Me;

	UPROPERTY()
	class UAwakenThorAnim* Anim;
	
	float IdleDelayTime = 3.f;
	float CurrentTime = 3.f;
	
	float TeleportDist = 1000;

	FTimerHandle MoveTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bPlay;

	TArray<std::pair<FVector, float>> AttackZone;

	bool bJump;

	

	void SphereOverlap(EHitType HitType, bool IsMelee);
};
