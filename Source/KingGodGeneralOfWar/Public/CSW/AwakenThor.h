// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AwakenThor.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API AAwakenThor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAwakenThor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LightBPClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> PoundCameraShake;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UProjectileMovementComponent* GetMjolnirMoveComp() const;
	UStaticMeshComponent* GetMjolnir() const;

	void Move(FVector NewLoc);

	UFUNCTION(BlueprintCallable)
	void ThrowForTeleport(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void Teleport(FVector Target);

	void EquipWeapon();
	void DrawWeapon();

	class UAwakenThorFSM* getFSM() const;
	bool SetHp(float Damage);

	void UpdateHpUI();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UAwakenThorFSM* Fsm;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UStaticMeshComponent* Mjolnir;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	class UProjectileMovementComponent* MjolnirMoveComp;
	
	UFUNCTION()
	void SetThorLocation(FVector NewLoc);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHpBar> HpBarBPClass;

	
	UPROPERTY()
	UHpBar* HpBar;

	float MaxHp = 100.f;
	float Hp = MaxHp;

	
};
