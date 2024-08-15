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

	void GetHitDirectionString(EAttackDirectionType AtkDir, FString& Str);
	void OnEnd();
	
	void SetGlobalTimeDilation(float Duration, float SlowScale);
	bool SetDamage(float Damage = 1, EAttackDirectionType AtkDir = EAttackDirectionType::UP, bool bSuperAttack = false);
	void SetJump(bool Value);
	void SetSuperArmor(bool Value);
	EAwakenThorState GetState() const;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=FSM, meta=(AllowPrivateAccess))
	EAwakenThorState State = EAwakenThorState::Idle;
	
	UPROPERTY(VisibleAnywhere, Category=FSM)
	AKratos* Target;
	
	UPROPERTY()
	class AAwakenThor* Me;

	UPROPERTY()
	class UAwakenThorAnim* Anim;

	UPROPERTY()
	class ACSWGameMode* GameMode;
	
	float IdleDelayTime = 1.5f;
	float CurrentTime = 0.f;
	
	float ArmorGage = 0.f;
	
	float TeleportDist = 1000;

	FTimerHandle MoveTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bPlay;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	TArray<UNiagaraSystem*> ThunderVFXarr;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	UNiagaraSystem* ThunderVFX;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	UNiagaraSystem* ClapVFX;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	UNiagaraSystem* KickVFX;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	UNiagaraSystem* EmberVFX;
	
	TArray<std::pair<FVector, float>> AttackZone;

	bool bJump;
	bool bSuperArmor = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess), Category="ZoneRadius")
	float ClapZoneRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess), Category="ZoneRadius")
	float PoundZoneRadius = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess), Category="ZoneRadius")
	float JumpAtkZoneRaidus = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess), Category="ZoneRadius")
	float KickZoneRadius = 100.f;

	

	UPROPERTY()
	TArray<UDecalComponent *> DecalZone;
	
	void SphereOverlap(const std::pair<FVector, float>& Zone, float Damage, EHitType HitType, bool IsMelee);
	UDecalComponent* DrawAttackZoneDecal(const std::pair<FVector, float>& Zone);

	void SpawnRandomThunderVFX(const FVector& Location);
	void SpawnThunderVFX(const FVector& Location);
	


	float getFloorZ(FVector2D Loc);
};
