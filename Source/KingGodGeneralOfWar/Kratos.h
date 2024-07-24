// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kratos.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{	Attack1 UMETA(DisplayName = "Attack1"),
	Attack2 UMETA(DisplayName = "Attack2"),
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

	void Damage(int DamageValue, EAttackType AttackType);

};
