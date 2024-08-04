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

	UPROPERTY(EditDefaultsOnly)
	class AKratos* Target; //타겟


	// 나는 무기, 나를 가진 주인
	UPROPERTY(EditDefaultsOnly)
	class ABDThor* Thor; //나 자신

	UPROPERTY(EditDefaultsOnly)
	int32 BDThrowDamage = 10;


	UFUNCTION()
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
	void BackMjolnir(); //묠니르 발사 후 돌아오는 함수

	bool bReturning; //BDHammerThrow인 상태
	bool bCreateTrue; //false일때는 생성 가능, true 일때는 생성 불가능


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
