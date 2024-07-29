// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Kratos.generated.h"

const float PlayerMaxSpeed = 1200.0f; // 플레이어 최대 속도. (달리기)

const int8 MaxCombo = 4;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Run UMETA(DisplayName = "Run"),
	Dodge UMETA(DisplayName = "Dodge"),
	Roll UMETA(DisplayName = "Roll"),
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	MeleeAttack2 UMETA(DisplayName = "MeleeAttack2"),
	MeleeAttack3 UMETA(DisplayName = "MeleeAttack3"),
	MeleeAttack4 UMETA(DisplayName = "MeleeAttack4"),
	DashAttack UMETA(DisplayName = "DashAttack"),
	GuardStart UMETA(DisplayName = "GuardStart"),
	Guard UMETA(DisplayName = "Guard"),
	GuardHit UMETA(DisplayName = "GuardHit"),
	GuardStaggerd UMETA(DisplayName = "GuardStaggerd"),
	GuardEnd UMETA(DisplayName = "GuardEnd"),
	Hit UMETA(DisplayName = "Hit"),
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Attack1 UMETA(DisplayName = "Attack1")
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
	class UInputAction* IA_Attack;

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
	void OnMyActionAttack(const FInputActionValue& value);

	void AttackStartComboState();
	void AttackEndComboState();

	UFUNCTION()
	void Damage(int DamageValue, EAttackType AttackType);

	FString GetPlayerStateString();
	void PlayerMove();

	UPROPERTY(BlueprintReadOnly)
	FVector Direction;

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

	void LockTargetFunc();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bIsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int CurrentCombo;

private:
	bool bIsAttacking;
	bool bIsDodging;
	FTimerHandle DodgeHandle;

	float CurrentTime;
	float DelayTime;

	FRotator TargetCameraRotation;
};
