// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "Kratos.generated.h"

const float PlayerMaxSpeed = 900.0f; // 플레이어 최대 속도. (달리기)
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Run UMETA(DisplayName = "Run"),
	Dodge UMETA(DisplayName = "Dodge"),
	Roll UMETA(DisplayName = "Roll"),
	Attack UMETA(DisplayName = "Attack"),
	Aim UMETA(DisplayName = "Aim"),
	GuardStart UMETA(DisplayName = "GuardStart"),
	Parry UMETA(DisplayName = "Parry"),
	Guard UMETA(DisplayName = "Guard"),
	GuardHit UMETA(DisplayName = "GuardHit"),
	GuardEnd UMETA(DisplayName = "GuardEnd"),
	NoneMovable UMETA(DisplayName = "NoneMovable"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die"),

};

UENUM(BlueprintType)
enum class EHitType : uint8
{
	STAGGER UMETA(DisplayName = "STAGGER"),
	NB_HIGH UMETA(DisplayName = "NB_HIGH"),
	NB_MEDIUM UMETA(DisplayName = "STAGGER"),
	NB_LOW UMETA(DisplayName = "NB_LOW"),
	STUN UMETA(DisplayName = "STUN"),
};

UENUM()
enum class EAttackDirectionType : uint8
{
	FORWARD = 0 UMETA(DisplayName = "FORWARD"),
	BACKWARD UMETA(DisplayName = "BACKWARD"),
	LEFT UMETA(DisplayName = "LEFT"),
	RIGHT UMETA(DisplayName = "RIGHT"),
};

const EAttackDirectionType AttackTypeDirectionArr[5][5] = {
	{EAttackDirectionType::FORWARD, EAttackDirectionType::RIGHT, EAttackDirectionType::LEFT, EAttackDirectionType::LEFT, },
	{EAttackDirectionType::FORWARD, EAttackDirectionType::RIGHT, EAttackDirectionType::LEFT, EAttackDirectionType::FORWARD, EAttackDirectionType::FORWARD},
	{EAttackDirectionType::FORWARD, EAttackDirectionType::LEFT, EAttackDirectionType::RIGHT, EAttackDirectionType::FORWARD, EAttackDirectionType::FORWARD},
	{EAttackDirectionType::FORWARD, EAttackDirectionType::BACKWARD, }
};
UENUM()
enum class EAttackType : uint8
{
	WEAK_ATTACK = 0 UMETA(DisplayName = "WEAK_ATTACK"),
	STRONG_ATTACK UMETA(DisplayName = "STRONG_ATTACK"),
	RUNE_ATTACK UMETA(DisplayName = "RUNE_ATTACK"),
	AXE_THROW_ATTACK UMETA(DisplayName = "AXE_THROW_ATTACK"),
	DASH_ATTACK UMETA(DisplayName = "DASH_ATTACK"),
};
UCLASS()
class KINGGODGENERALOFWAR_API AKratos : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKratos();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnMyActionMove(const FInputActionValue& Value);

	UFUNCTION()
	void OnMyActionLook(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionDodge(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionRunOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionRunOff(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionGuardOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionGuardOff(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionLockOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionIdle(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionWeakAttack(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionStrongAttack(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionAimOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionAimOff(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionWithdrawAxe(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionRuneBase(const FInputActionValue& value);

	UFUNCTION()
	void OnMontageEndedDelegated(UAnimMontage* Montage, bool bInterrupted);

	// Axe Throwing
	void OnHideAxe();
	void ThrowAxe(FRotator TargetRot);
	void WithdrawAxe();
	void CatchFlyingAxe();

	// Damage Function
	void Damage(int DamageValue, EHitType HitType, bool IsMelee);
	void Damage(AActor* Attacker, int DamageValue, EHitType HitType, bool IsMelee);

	void CameraShakeOnAttack(float scale = 1.0f);
	FString GetPlayerStateString();
	EAttackDirectionType GetAttackDirection();

private:
	void SetState(EPlayerState NextState);

	// Weak Attack Combo
	void WeakAttackStartComboState();
	void WeakAttackEndComboState();

	// Strong Attack Combo
	void StrongAttackStartComboState();
	void StrongWeakAttackEndComboState();

	void PlayerMove();
	void SetWeapon();
	void SetShield();
	void LockTargetFunc(float DeltaTime);

	FString GetHitSectionName(EHitType hitType);
	FString GetDodgeDirection(int& DodgeScale);

public:
	// Player Input
	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Guard;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_LockOn;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_WeakAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_StrongAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Aim;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_WithdrawAxe;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_RuneBase;

	// UClass Pointer
	class AActor* LockTarget;

	UPROPERTY()
	class USG_KratosAnim* Anim ;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* WithdrawPositionComp;

	// Kratos Weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AAxe> AxeFactory;

	UPROPERTY()
	class AAxe* Axe;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ASG_Shield> ShieldFactory;

	UPROPERTY()
	class ASG_Shield* Shield;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AFlyingAxe> FlyingAxeFactory;

	UPROPERTY()
	class AFlyingAxe* FlyingAxe;

	// VFX & Camera Shake
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> AttackShakeFactory;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UParticleSystem* ParryVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX Actor")
	TSubclassOf<class AActor> ParryingLightFactory;

	UPROPERTY(EditDefaultsOnly, Category = "VFX Actor")
	TSubclassOf<class AActor> GuardBlockLightFactory;

	// UI
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> HpBarUIFactory;

	UPROPERTY()
	class UPlayerHPUI* HpBarUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> AimWidgetClass;
	UPROPERTY()
	class UUserWidget* AimWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerState State = EPlayerState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float MinPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float MaxPitch;

	// 룬공격 막타 줌아웃
	bool bZoomOut;

	EAttackType CurrentAttackType;
	
private:
	
	FRotator TargetCameraRotation;
	FRotator TargetActorRotation;

	FVector Direction;
	FVector PrevDirection;

	float TargetFOV = 90;

	bool bLockOn;
	bool bIsAttacking;
	bool bIsDodging;
	bool bParrying;
	bool bRuneReady;
	bool bGuardStagger;
	bool bSuperArmor;

	/// Attack Combo 를 위한 bool 변수 및 콤보 카운트
	bool CanNextStrongCombo;
	bool bIsStrongComboInputOn;
	int CurrentStrongCombo;

	bool CanNextWeakCombo;
	bool bIsWeakComboInputOn;
	int CurrentWeakCombo;

	bool bAxeGone;
	bool bIsAxeWithdrawing;

	int GuardHitCnt;
};
