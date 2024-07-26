// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BDThorFSM.generated.h"

//전체 통괄 상태
//피격은 얼라이언스 / Attack, Move은 열거형으로 관리
UENUM(BlueprintType)
enum class BDThorGeneralState : uint8 {
	BDIdle,
	BDMove,
	BDAvoidance,
	BDAttack,
	BDDamage,
};

//공격 상태 관리 열거형
UENUM(BlueprintType)
enum class BDThorAttack : uint8 {
	BDHammerThunder,
	BDHammerWind,
	BDHammerThreeSwing,
	BDGiveUPFly,
	BDHittingDown,
};

//이동 상태 관리 열거형
UENUM(BlueprintType)
enum class BDThorMove : uint8 {
	BDBackMove,
	BDFrontMove,
	BDRightMove,
	BDLeftMove,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGGODGENERALOFWAR_API UBDThorFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBDThorFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditAnywhere)
	class ABDThor* me; //나 자신

	UPROPERTY(EditAnywhere, Category = FSM) 
	float BDAttackRange = 50.0f; // 에너미 공격 범위
	

	UPROPERTY(EditAnywhere)
	//class AKratos* Target; //타겟
	class ACharacter* Target;

	//상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState mState = BDThorGeneralState::BDIdle;


	// 총 상태 총괄 함수
	void BDIdleState(); //대기 상태
	void BDMoveState(); //움직임 상태
	void BDAvoidanceState(); //회피 상태
	void BDAttackState(); //공격 상태
	void BDDamageState(); //피격 상태

	//대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDidleDelayTime = 2.0f;
	//경과 시간
	float BDCurrentTime = 0;

	//공격 대기 시간, 토르는 공격하는 딜레이 시간이 다양하다 랜덤 쓸것
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDAttackDelayTime = 2.0f;

	//피격 시 데미지 함수, DamageNum 데미지 수치
	UFUNCTION()
	void Damage(float DamageNum);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDMaxHp = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDCurrentHP;

	//공격 함수

		
};
