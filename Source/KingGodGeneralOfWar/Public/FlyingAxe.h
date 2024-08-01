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
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SubMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* DirectionArrowComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* RotationAxisComp;

	float MoveSpeed = 2000.0f;
	float RotationSpeed = 90.0f;
};
