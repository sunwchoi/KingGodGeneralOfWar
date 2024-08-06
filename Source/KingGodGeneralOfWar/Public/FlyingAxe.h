// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyingAxe.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API AFlyingAxe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyingAxe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SubMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* DirectionArrowComp;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* HitArrowComp;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* WithdrawTargetPosition;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* WithdrawRotation;

	UPROPERTY()
	class AKratos* Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* BloodVFXFactory;

	UFUNCTION()
	void FlyingAxeOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void BackToPlayer();

	FVector TargetLocation;
	FVector CurLocation;

	float MoveSpeed = 4000.0f;
	float RotationSpeed = 90.0f;
	bool bHit;
	bool bWithdrawing;
	bool bRising;
	float LerpAlpha;
	float WithdrawRotationScale = -20;

};
