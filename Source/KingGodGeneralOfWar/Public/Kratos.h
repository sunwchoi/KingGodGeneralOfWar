// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Kratos.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Run UMETA(DisplayName = "Run"),
	Roll UMETA(DisplayName = "Roll"),
	Attack UMETA(DisplayName = "Attack"),
	Guard UMETA(DisplayName = "Guard"),
	Hit UMETA(DisplayName = "Hit")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Roll;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Guard;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_LockOn;

	UFUNCTION()
	void OnMyActionMove(const FInputActionValue& Value);

	UFUNCTION()
	void OnMyActionLook(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionRoll(const FInputActionValue& value);

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
	void OnMyActionLockOff(const FInputActionValue& value);

	UFUNCTION()
	void ExitRolling();

	UFUNCTION()
	void Damage(int DamageValue, EAttackType AttackType);

	FString GetEnumValueAsString();
	void PlayerMove();

	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLockOnTarget;

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
};
