// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BDThor.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API ABDThor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABDThor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	
	//적 관리 컴포넌트 클래스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UBDThorFSM* fsm; //토르 FSM


	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = weapon)
	class UStaticMeshComponent* BDWeapon;*/

	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABDThorMjolnir> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABDThorMjolnir> CurrentWeapon;

	//공격할 때 손으로 위치 바꾸기
	UFUNCTION()
	void EquipWeapon();

	//바람 공격할 때 넣을 액터 클래스
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class ASlashActor>SlashFat;*/

	//망치 날릴때 쓸 공장
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABDThorMjolnir> MjolnirFactory;

	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThrowHit(); //망치를 Fire 하는 능력
};
