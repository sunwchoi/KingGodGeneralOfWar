// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "Kratos.generated.h"

const float PlayerMaxSpeed = 900.0f; // 플레이어 최대 속도. (달리기)
const FVector DefaultCameraOffset = FVector(0, 50, 70);
const FRotator DefaultCameraAngle = FRotator(0);
const float DefaultTargetTargetArmLength = 147;
const float DefaultTargetFOV = 90;

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
	DOWN = 0 UMETA(DisplayName = "DOWN"),
	UP  UMETA(DisplayName = "UP"),
	LEFT UMETA(DisplayName = "LEFT"),
	RIGHT UMETA(DisplayName = "RIGHT"),
};

const EAttackDirectionType AttackTypeDirectionArr[5][5] = {
	/*WeakCombo*/	{EAttackDirectionType::DOWN, EAttackDirectionType::RIGHT, EAttackDirectionType::LEFT, EAttackDirectionType::LEFT,},
	/*StrongCombo*/	{EAttackDirectionType::DOWN, EAttackDirectionType::RIGHT, EAttackDirectionType::LEFT, EAttackDirectionType::UP, EAttackDirectionType::DOWN},
	/*RuneAttack*/	{EAttackDirectionType::DOWN, EAttackDirectionType::LEFT, EAttackDirectionType::RIGHT, EAttackDirectionType::DOWN, EAttackDirectionType::UP},
	/*AxeThrow*/	{EAttackDirectionType::DOWN, EAttackDirectionType::UP,}
};

UENUM()
enum class EAttackType : uint8
{
	WEAK_ATTACK = 0 UMETA(DisplayName = "WEAK_ATTACK"),
	STRONG_ATTACK UMETA(DisplayName = "STRONG_ATTACK"),
	RUNE_ATTACK UMETA(DisplayName = "RUNE_ATTACK"),
	AXE_THROW_ATTACK UMETA(DisplayName = "AXE_THROW_ATTACK"),
	DASH_ATTACK UMETA(DisplayName = "DASH_ATTACK"),
	NONE UMETA(DisplayName = "NONE"),
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

	void OnMyGuardDisappear();

	void OnMyLaunchCharacterInStrongAttack();
	void OnMyJumpCharacterInStrongAttack();

	void IncreaseTargetTargetArmLength(float value);
	void IncreaseTargetCameraOffset(FVector value);

	// Axe Throwing
	void OnHideAxe();
	void ThrowAxe(FRotator TargetRot);
	void WithdrawAxe();
	void CatchFlyingAxe();

	// Damage Function
	// if Kratos get damage, return true; else return false;
	bool Damage(AActor* Attacker, int DamageValue, EHitType HitType, bool IsMelee);
	void LaunchKratos(float LaunchScale = 2000);

	// Rune
	void OnMyRuneReady();
	void OnMyRuneAttackEnd();
	void OnMyRuneAttackCameraSet();
	void OnMySpawnEarthCrack();

	void OnMyAttackComboEnd();
	void OnMyInitAttackType();

	void OnMyAttackProgress();

	void OnMyEndWithFail();
	void OnMyGetUPCameraSet();

	void CameraShakeOnAttack(EAttackDirectionType attackDir = EAttackDirectionType::UP, float scale = 1.0f);
	FString GetPlayerStateString();
	EAttackDirectionType GetAttackDirection();
	int32 GetCurrentWeakCombo();
	int32 GetCurrentStrongCombo();
	void SetGlobalTimeDilation(float Duration, float SlowScale);
private:
	void SetState(EPlayerState NextState);

	// Weak Attack Combo
	void WeakAttackStartComboState();
	void WeakAttackEndComboState();

	// Strong Attack Combo
	void StrongAttackStartComboState();
	void StrongAttackEndComboState();


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
	class USG_KratosAnim* Anim;

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
	TSubclassOf<class UCameraShakeBase> DownAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> UpAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> LeftAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> RightAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")

	TSubclassOf<class UCameraShakeBase> CatchAxeShakeFactory;

	UPROPERTY()
	TArray< TSubclassOf<class UCameraShakeBase>> AttackShakeFactoryArr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* ParryVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* ShockWaveVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* GuardBlockVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* GuardCrashVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* EarthCrackVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX Actor")
	TSubclassOf<class AActor> ParryingLightFactory;

	UPROPERTY(EditDefaultsOnly, Category = "VFX Actor")
	TSubclassOf<class AActor> GuardBlockLightFactory;

	// ------------------------------------------ SFX -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* AvoidSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* AxeThrowSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* AxeWithdrawSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* HitSound1;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* HitSound2;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* HitSound3;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RollSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneBaseSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack1Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack2Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack3Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack4Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack1Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack2Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack3Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack4Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* WeakAttack1Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* WeakAttack2Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* WeakAttack3Sound;

	UPROPERTY()
	TArray<class USoundBase*> WeakAttackSoundArr;

	UPROPERTY()
	TArray<class USoundBase*> StrongAttackSoundArr;

	UPROPERTY()
	TArray<class USoundBase*> RuneAttackSoundArr;

	UPROPERTY()
	class ACSWGameMode* GameMode;

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
	float TargetGuardScale = 0;
	FVector Direction;
	FVector PrevDirection;

	float TargetFOV = DefaultTargetFOV;
	float TargetTargetArmLength = DefaultTargetTargetArmLength;
	FVector TargetCameraOffset = DefaultCameraOffset;
	FRotator TargetCameraAngle = FRotator(0);

	bool bLockOn;
	bool bIsAttacking;
	bool bIsDodging;
	bool bParrying = false;
	bool bRuneReady;
	bool bGuardStagger = false;
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
