// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BDThorMjolnir.h"
#include "BDThorFSM.generated.h"

//전체 통괄 상태
//피격은 얼라이언스 / Attack, Move은 열거형으로 관리
//공격, 회피도 이곳에서 관리
UENUM(BlueprintType)
enum class BDThorGeneralState : uint8 {
	BDIdle,
	BDMove,
	BDAvoidance,
	BDAttackModeChange,
	BDDamage,
	BDHammerThrow,
	BDHammerWind,
	BDHammerThreeSwing,
	BDGiveUPFly,
	BDHitDown,
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


	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDAttackRange = 300.0f; // 에너미 공격 범위
	

	UPROPERTY(EditDefaultsOnly)
	class AKratos* Target; //타겟
	//class ACharacter* Target;

	//상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState mState = BDThorGeneralState::BDIdle;


	// 총 상태 총괄 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void BDIdleState(); //대기 상태
	UFUNCTION(BlueprintCallable, Category = State)
	void BDMoveState(); //움직임 상태
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAvoidanceState(); //회피 상태
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAttackModeChangeState(); //공격 모드 변경 상태
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	BDThorGeneralState RandomAttackState(); // 랜덤 공격 상태 선택 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void BDDamageState(); //피격 상태

	//공격 함수
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThrowState(); //망치 날리면서 공격
	//UFUNCTION(BlueprintCallable, Category = Attack)
	//void BDHammerThrowHit(); //망치를 Fire 하는 능력

	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerWindState(); //망치 휘두르면서 바람날리는 공격
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThreeSwingState(); //망치 n번 휘두르기

	//근접 컷씬 공격 함수
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDGiveUPFlyState();
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDHittingDownState();


	UPROPERTY(EditDefaultsOnly)
	BDThorGeneralState LastAttackState; // 마지막 공격 상태를 저장하는 변수


	//사용중인 애니메이션 블루프린트
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Anim)
	class UBDThorAnim* anim;

	//애니메이션 총관리 함수, 노티파이를 넣어서 이 함수를 호출한다. 이 함수들을 이용해 스테이트를 관리한다.
	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDEndState();

	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDSetState(BDThorGeneralState BDnewState);

	//대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDidleDelayTime = 0.5f;
	//경과 시간
	float BDCurrentTime = 0;

	//공격 랜덤을 선택하기 전의 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDAttackDelayTime = 0.3f;

	//토르는 공격하는 딜레이 시간이 다양하다 랜덤 쓸것
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDDelayTime = 1.0f;

	//피격 시 데미지 함수, DamageNum 데미지 수치
	UFUNCTION()
	void Damage(float DamageNum);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDMaxHp = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDCurrentHP;

	UPROPERTY(EditDefaultsOnly)
	FTimerHandle StateTimerHandle; //상태 변경 전의 딜레이 주기 위한 변수
		
};
