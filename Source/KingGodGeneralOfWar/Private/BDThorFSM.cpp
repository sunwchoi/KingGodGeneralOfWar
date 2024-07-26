// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorFSM.h"
#include "BDThor.h"
#include "Kratos.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UBDThorFSM::UBDThorFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UBDThorFSM::BeginPlay()
{
	Super::BeginPlay();

	//월드에서 플레이어 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	//AKratos 타입으로 캐스팅
	Target = Cast<AKratos>(actor);
	//ACharacter* tmp = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	/*if (tmp) {
		Target = tmp;
	}*/
	//소유 객체 가져오기
	me = Cast<ABDThor>(GetOwner());

	//체력 설정
	BDCurrentHP = BDMaxHp;

	
}


// Called every frame
void UBDThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState) {
		case BDThorGeneralState :: BDIdle:
			BDIdleState();
			break;
		case BDThorGeneralState :: BDMove:
			BDMoveState();
			break;
		case BDThorGeneralState::BDAttack:
			BDAttackState();
			break;
		case BDThorGeneralState::BDAvoidance:
			BDAvoidanceState();
			break;
		case BDThorGeneralState :: BDDamage:
			BDDamageState();
			break;
	}

	//실행창에 상태 메세지 출력
	FString logMessege = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMessege);
}

//상태 관리 함수
void UBDThorFSM::BDIdleState()
{
	//시간이 흐르면서
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//만약 경과 시간이 대기 시간을 초과했다면
	if (BDCurrentTime > BDidleDelayTime) {
		//3. 이동 상태로 전환하고 싶다.
		mState = BDThorGeneralState::BDMove;
		//경과 시간 초기화
		BDCurrentTime = 0;
	}
}

void UBDThorFSM::BDMoveState()
{
	//기본적으로는 플레이어를 향해 이동하지만
	//피격 상태가 되었거나 공격을 하기 전에 플레이어를 바라보며 뒤로 이동하거나 옆으로 회피를 한다.

	//기본 이동 구현
	//1. 타깃 목적지가 필요하다.
	FVector destination = Target->GetActorLocation();
	//2. 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();
	//3. 방향으로 이동하고 싶다.
	me->AddMovementInput(dir.GetSafeNormal());

	//타깃과 가까워지면 공격 상태로 전환하고 싶다.
	//1. 만약 거리가 공격 범위 안에 들어오면
	if (dir.Size() < BDAttackRange) {
		//2. 공격 상태로 전환하고 싶다.
		//여기 더 구체화
		mState = BDThorGeneralState::BDAttack;
	}
}

void UBDThorFSM::BDAvoidanceState()
{
	//회피 구현
	//플레이어의 방향을 중점으로 뒤로 피하거나 옆으로 피한다.
}



void UBDThorFSM::BDAttackState()
{
	//일정시간에 다양한 패턴으로 넘어가야한다.
	//일단 간단 구현
	//1. 시간이 흐르고
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//2. 공격 시간이 되면
	if (BDCurrentTime > BDAttackDelayTime) {
		//공격, 여기엔 다양한 공격 타입을 랜덤으로 설정해둔다.
 		UE_LOG(LogTemp, Warning, TEXT("player BDAttack!!"));
		//경과 시간 초기화 후 대기 또는 이동으로 상태 변경하기
		BDCurrentTime = 0;

		//mState = BDThorGeneralState::BDIdle;  // 상태를 대기로 전환
	}

	//타깃이 공격을 받으면 다시 이동으로 변하고 싶다.

	//임시 코드, 플레이어의 Damage가 발생했을 경우 에너미의 상태를 대기 또는 이동으로 변경
	float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	//타깃의 거리가 공격 범위를 벗어났으니까
	if (distance > BDAttackRange) {
		//상태를 잠시 대기로 전환한다.
		mState = BDThorGeneralState::BDMove;
	}

}

void UBDThorFSM::BDDamageState()
{
	//피격 애니메이션 재생

	Damage(10); //데미지 


	//체력이 남아있다면
	if (BDCurrentHP > 0) {
		//상태를 회피로 전환
		mState = BDThorGeneralState::BDMove;
	}
	else if (BDCurrentHP <= 0) {
		//2 페이즈로 전환
	}
}

void UBDThorFSM::BDEndState()
{

}

void UBDThorFSM::SetState()
{
}

//데미지를 받을 시 발생하는 함수
void UBDThorFSM::Damage(float DamageNum)
{
	BDCurrentHP -= DamageNum;
	UE_LOG(LogTemp, Warning, TEXT("BDThor damage!"));

}

