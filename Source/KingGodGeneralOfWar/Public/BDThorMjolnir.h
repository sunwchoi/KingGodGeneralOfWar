// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDThorMjolnir.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API ABDThorMjolnir : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABDThorMjolnir();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> MjolnirMesh; //메쉬

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UCapsuleComponent> MjoCol; //충돌체

	//발사체 컴포넌트
	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComp;

};
