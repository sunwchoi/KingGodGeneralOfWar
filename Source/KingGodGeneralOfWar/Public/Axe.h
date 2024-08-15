// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Axe.generated.h"


UCLASS()
class KINGGODGENERALOFWAR_API AAxe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAxe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX1Factory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX2Factory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX3Factory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX4Factory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX5Factory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX6Factory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX7Factory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	class UNiagaraSystem* BloodVFX8Factory;


	UPROPERTY()
	TArray<class UNiagaraSystem*> BloodVFXFactoryArr;

	UFUNCTION()
	void OnAxeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* EdgeComp;

	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY()
	class AKratos* Me;
};
