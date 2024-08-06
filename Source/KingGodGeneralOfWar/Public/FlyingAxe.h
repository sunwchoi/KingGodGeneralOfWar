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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SubMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* DirectionArrowComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* HitArrowComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* WithdrawTargetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* WithdrawRotation;

	float MoveSpeed = 4000.0f;
	float RotationSpeed = 90.0f;

	UFUNCTION()
	void FlyingAxeOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector TargetLocation;
	FVector CurLocation;

	bool bHit;
	bool bWithdrawing;
	bool bRising;
	float LerpAlpha;
	float WithdrawRotationScale = -20;


	void BackToPlayer();

	UPROPERTY()
	class AKratos* Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* BloodVFXFactory;

};
