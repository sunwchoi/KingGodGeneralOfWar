// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BDThorMjolnir.h"
#include "BDThorFSM.generated.h"

//��ü ��� ����
//�ǰ��� ����̾� / Attack, Move�� ���������� ����
//����, ȸ�ǵ� �̰����� ����
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


//�̵� ���� ���� ������
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
	class ABDThor* me; //�� �ڽ�


	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDAttackRange = 300.0f; // ���ʹ� ���� ����
	

	UPROPERTY(EditDefaultsOnly)
	class AKratos* Target; //Ÿ��
	//class ACharacter* Target;

	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState mState = BDThorGeneralState::BDIdle;


	// �� ���� �Ѱ� �Լ�
	UFUNCTION(BlueprintCallable, Category = State)
	void BDIdleState(); //��� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDMoveState(); //������ ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAvoidanceState(); //ȸ�� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAttackModeChangeState(); //���� ��� ���� ����
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	BDThorGeneralState RandomAttackState(); // ���� ���� ���� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = State)
	void BDDamageState(); //�ǰ� ����

	//���� �Լ�
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThrowState(); //��ġ �����鼭 ����
	//UFUNCTION(BlueprintCallable, Category = Attack)
	//void BDHammerThrowHit(); //��ġ�� Fire �ϴ� �ɷ�

	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerWindState(); //��ġ �ֵθ��鼭 �ٶ������� ����
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThreeSwingState(); //��ġ n�� �ֵθ���

	//���� �ƾ� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDGiveUPFlyState();
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDHittingDownState();

	//���� ���� �� �뽬
	UFUNCTION()
	void BDDash();

	//��ġ �ֵθ��⿡�� ���� üũ
	bool bBDAttackCheck;


	UPROPERTY(EditDefaultsOnly)
	BDThorGeneralState LastAttackState; // ������ ���� ���¸� �����ϴ� ����


	//������� �ִϸ��̼� �������Ʈ
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Anim)
	class UBDThorAnim* anim;

	//�ִϸ��̼� �Ѱ��� �Լ�, ��Ƽ���̸� �־ �� �Լ��� ȣ���Ѵ�. �� �Լ����� �̿��� ������Ʈ�� �����Ѵ�.
	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDEndState();

	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDSetState(BDThorGeneralState BDnewState);

	//��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDidleDelayTime = 0.7f;
	//��� �ð�
	float BDCurrentTime = 0;

	//���� ������ �����ϱ� ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDAttackDelayTime = 0.3f;

	//�丣�� �����ϴ� ����� �ð��� �پ��ϴ� ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDDelayTime = 0.5f;

	//�ǰ� �� ������ �Լ�, DamageNum ������ ��ġ
	UFUNCTION()
	void Damage(float DamageNum, EAttackDirectionType AtkDir);

	UFUNCTION()
	void BDGetHitDirectionString(EAttackDirectionType AtkDir);

	UPROPERTY()
	FString Str; //������ ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDMaxHp = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDCurrentHP;
		
};
