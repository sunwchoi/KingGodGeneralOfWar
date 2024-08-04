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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = weapon)
	class UStaticMeshComponent* BDWeapon; //무기 컴포넌트

	//공격할 때 손으로 위치 바꾸기
	UFUNCTION()
	void EquipWeapon();

	bool IsWeaponHold; //무기가 손에 잇을때 true

	//망치 공격이 아닐땐 항상 허리춤에 망치 달기
	UFUNCTION()
	void DrawWeapon();

	UFUNCTION()
	void EquipRight();

	UFUNCTION()
	void HiddenWeapon(); //무기 어디서도 안보이게 하기 : 무기를 던졌을 때 호출
	void visibleWeapon(); //무기를 안보이게 했으면 다시 보이게 하기 : 무기 던지고 난 뒤에 호출

	//바람 공격할 때 넣을 액터 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWindSlash> SlashFat;

	//망치 날릴때 쓸 공장
	UPROPERTY(EditDefaultsOnly, Category = weapon)
	TSubclassOf<class ABDThorMjolnir> MjolnirFactory;

	UPROPERTY()
	class ABDThorMjolnir* FlyWeapon;


	UFUNCTION(BlueprintCallable, Category = weapon)
	void BDHammerThrowHit(); //망치를 Fire 하는 능력

	//바람을 날려서 데미지 주기
	UFUNCTION(BlueprintCallable, Category = weapon)
	void BDHammerWindSlash(); //바람 날리는 능력


};
