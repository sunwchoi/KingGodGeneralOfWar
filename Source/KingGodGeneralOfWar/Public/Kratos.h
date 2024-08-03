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
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	Aim UMETA(DisplayName = "Aim"),
	DashAttack UMETA(DisplayName = "DashAttack"),
	GuardStart UMETA(DisplayName = "GuardStart"),
	Guard UMETA(DisplayName = "Guard"),
	GuardHit UMETA(DisplayName = "GuardHit"),
	GuardStaggerd UMETA(DisplayName = "GuardStaggerd"),
	GuardEnd UMETA(DisplayName = "GuardEnd"),
	Hit UMETA(DisplayName = "Hit"),
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
	UPROPERTY()
	class USG_KratosAnim* Anim ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Dodge;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Guard;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_LockOn;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_WeakAttack;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_StrongAttack;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Aim;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_WithdrawAxe;

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

	// 약공격 콤보
	void WeakAttackStartComboState();
	void WeakAttackEndComboState();

	// 강공격 콤보
	void StrongAttackStartComboState();
	void StrongWeakAttackEndComboState();

	UFUNCTION()
	void Damage(int DamageValue, EHitType HitType, bool IsMelee);

	FString GetPlayerStateString();
	void PlayerMove();

	UPROPERTY(BlueprintReadOnly)
	FVector Direction;
	UPROPERTY(BlueprintReadOnly)
	FVector PrevDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLockOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* LockTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerState State = EPlayerState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float MinPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float MaxPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thor");
	TSubclassOf<class ABDThor> Boss1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thor");
	TSubclassOf<class ABDThor> Boss2;

	void LockTargetFunc(float DeltaTime);

	UFUNCTION()
	void OnStrongAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnWeakAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnGuardMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextWeakCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bIsWeakComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int CurrentWeakCombo;

	/// <summary>
	/// 강공격 콤보를 위한 bool 변수 및 콤보 카운트
	/// </summary>
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextStrongCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bIsStrongComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))

	int CurrentStrongCombo;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAxe> AxeFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASG_Shield> ShieldFactory;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AAxe* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ASG_Shield* Shield;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> AttackShake;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> AimWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class UUserWidget* AimWidget;

	void SetWeapon();
	void SetShield();
	void CameraShakeOnAttack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* ParryVFX;

	bool bAxeGone;
private:
	bool bIsAttacking;
	bool bIsDodging;
	FTimerHandle DodgeHandle;

	float CurrentTime;
	float DelayTime;

	FRotator TargetCameraRotation;
	FRotator TargetActorRotation;
	float TargetFOV = 90;

	int guardHitCnt = 3;
	bool bParrying;
};
