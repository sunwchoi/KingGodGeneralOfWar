// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorFSM.h"
#include "BDThor.h"
#include "Kratos.h"
#include "BDThorAnim.h"
#include "Kismet/GameplayStatics.h"
#include "BDThorMjolnir.h"


// Sets default values for this component's properties
UBDThorFSM::UBDThorFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	LastAttackState = BDThorGeneralState::BDIdle; // 초기화
}


// Called when the game starts
void UBDThorFSM::BeginPlay()
{
	Super::BeginPlay();

	//월드에서 플레이어 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	//AKratos 타입으로 캐스팅
	Target = Cast<AKratos>(actor);
	//소유 객체 가져오기
	me = Cast<ABDThor>(GetOwner());

	//애니메이션 할당
	anim = Cast<UBDThorAnim>(me->GetMesh()->GetAnimInstance());

	//체력 설정
	BDCurrentHP = BDMaxHp;

}


// Called every frame
void UBDThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState) {
	case BDThorGeneralState::BDIdle:
		BDIdleState(); //대기
		break;
	case BDThorGeneralState::BDMove:
		BDMoveState(); //움직임
		break;
	case BDThorGeneralState::BDAttackModeChange:
		BDAttackModeChangeState(); //공격 상태 변경 함수
		break;
	case BDThorGeneralState::BDAvoidance:
		BDAvoidanceState(); //회피
		break;
	case BDThorGeneralState::BDDamage:
		BDDamageState(); //피격
		break;
	case BDThorGeneralState::BDHammerThrow:
		BDHammerThrowState(); //공격 패턴
		break;
	case BDThorGeneralState::BDHammerWind:
		BDHammerWindState(); //공격 패턴
		break;
	case BDThorGeneralState::BDHammerThreeSwing:
		BDHammerThreeSwingState(); //공격 패턴
		break;
	case BDThorGeneralState::BDGiveUPFly:
		BDGiveUPFlyState(); //공격 패턴
		break;
	case BDThorGeneralState::BDHitDown:
		BDHittingDownState(); //공격 패턴
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

		//애니메이션 상태 동기화
		anim->animState = mState;
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


	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	if (BDCurrentTime > BDDelayTime) {
		mState = BDThorGeneralState::BDAttackModeChange;

		//애니메이션 상태 동기화
		anim->animState = mState;
		BDCurrentTime = 0;
	}

	//타깃과 가까워지면 공격 상태로 전환하고 싶다.
	//1. 만약 거리가 공격 범위 안에 들어오면
	//if (dir.Size() < BDAttackRange) {
	//	//2. 공격 상태로 전환하고 싶다.
	//	//여기 더 구체화
	//	mState = BDThorGeneralState::BDAttackModeChange;

	//	//애니메이션 상태 동기화
	//	anim->animState = mState;
	//}
}


void UBDThorFSM::BDAttackModeChangeState()
{
	//일정시간에 다양한 패턴으로 넘어가야한다.

	//우선은 애니메이션은 대기 or 움직임 or 회피 애니메이션을 재생한다.
	//우선은 대기 상태 애니메이션 재생으로 지정
	anim->playBDAttackRandomState();

	//1. 시간이 흐르고
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//2. 공격 시간이 되면
	if (BDCurrentTime > BDAttackDelayTime) {
		// 랜덤으로 공격 패턴 선택
		mState = RandomAttackState();
		//mState = BDThorGeneralState::BDHammerWind;  // 임시 상태
		anim->animState = mState;
		
		//UE_LOG(LogTemp, Warning, TEXT("AttackModeChangeState: %s"), *UEnum::GetValueAsString(mState));
		
		BDCurrentTime = 0;
	}

	//타깃이 공격을 받으면 다시 이동으로 변하고 싶다.

	////임시 코드, 플레이어의 Damage가 발생했을 경우 에너미의 상태를 대기 또는 이동으로 변경
	//float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	////타깃의 거리가 공격 범위를 벗어났으니까
	//if (distance > BDAttackRange) {
	//	//상태를 잠시 대기로 전환한다.
	//	mState = BDThorGeneralState::BDIdle;
	//	//애니메이션 상태 동기화
	//	anim->animState = mState;
	//}

}

//공격 패턴을 랜덤으로 지정하는 함수
BDThorGeneralState UBDThorFSM::RandomAttackState()
{
	//애니메이션 상태는 우선 몽타주를 실행한다
	
	// 가능한 상태들을 배열로 저장
	TArray<BDThorGeneralState> AttackStates = {
		BDThorGeneralState::BDHammerThrow,
		BDThorGeneralState::BDHammerWind,
		BDThorGeneralState::BDHammerThreeSwing,
		//BDThorGeneralState::BDGiveUPFly,
		//BDThorGeneralState::BDHitDown
	};

	// 마지막 상태 제거
	AttackStates.Remove(LastAttackState);

	// 랜덤으로 선택
	int32 RandomIndex = FMath::RandRange(0, AttackStates.Num() - 1);
	BDThorGeneralState NewState = AttackStates[RandomIndex];

	//만약 망치 공격일 경우
	if (NewState == BDThorGeneralState::BDHammerThrow || NewState == BDThorGeneralState::BDHammerWind) {
		//손에 망치를 들어라
		me->EquipWeapon();
	}
	else if(NewState == BDThorGeneralState::BDHammerThreeSwing) {
		me->EquipRight(); //오른손에 망치를 들어라
	}
	else {
		me->DrawWeapon(); //허리에 망치를 두어라
	}

	// 마지막 상태 업데이트
	LastAttackState = NewState;
	//UE_LOG(LogTemp, Warning, TEXT("Random!!"));
	return NewState; //상태 리턴
}

//데미지를 받을 시 발생하는 함수
void UBDThorFSM::Damage(float DamageNum)
{
	BDCurrentHP -= DamageNum;

	//피격 상태로 변경한다.
	mState = BDThorGeneralState::BDDamage;

	UE_LOG(LogTemp, Warning, TEXT("BDThor damage!"));

}

void UBDThorFSM::BDDamageState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorDamageMontage)) {
		UE_LOG(LogTemp, Warning, TEXT("Damage"));
		//피격 애니메이션 재생
		anim->playBDDamage();
	}

	//UE_LOG(LogTemp, Warning, TEXT("DamageState"));
	
}

void UBDThorFSM::BDAvoidanceState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorRightDodgeMontage)) {
		//데미지를 받았을 때 회피
		UE_LOG(LogTemp, Warning, TEXT("Avoidance"));
		anim->playBDRightDodge();
	}

	//이제 근처 물체 위치를 받아서 오른쪽으로 회피할 것인지 뒤쪽으로 회피할 것인지에 대한 코드 작성

	//플레이어의 방향을 중점으로 뒤로 피하거나 옆으로 피한다.
}


//망치 날리면서 번개 공격
void UBDThorFSM::BDHammerThrowState()
{
	// 현재 상태가 이미 Hammer Wind라면 함수 호출을 건너뛴다.
	if (mState != BDThorGeneralState::BDHammerThrow) {
		return;
	}

	if (!anim->Montage_IsPlaying(anim->BDHammerThrowMontage)) {
		anim->playBDHammerThrow();
	}

	
}

//망치 휘두르면서 바람공격
void UBDThorFSM::BDHammerWindState()
{
	if (me->IsWeaponHold == false) {
		me->EquipWeapon(); //우선 무기를 든다.
	}

	// 현재 상태가 이미 Hammer Wind라면 함수 호출을 건너뛴다.
	if (mState != BDThorGeneralState::BDHammerWind) {
		return;
	}

	// 애니메이션이 재생되고 있는지 확인하고, 재생되지 않았다면 재생한다.
	if (!anim->Montage_IsPlaying(anim->BDHammerWindMontage))
	{
		anim->playBDHammerWind();
		UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Wind Animation"));
	}

}

//망치 n번 휘두르기
void UBDThorFSM::BDHammerThreeSwingState()
{
	if (!anim->Montage_IsPlaying(anim->BDHammerThreeSwingMontage))
	{
		anim->playBDHammerThreeSwing(); //3번 휘두르기
		UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Three Swing Animation"));
	}
}

//플레이어 잡아서 업어치기 (카메라 컷씬)
void UBDThorFSM::BDGiveUPFlyState()
{
}

//바닥을 주먹으로 내려치기
void UBDThorFSM::BDHittingDownState()
{
}




//애니메이션 마지막 종료 후 상태 확인 
// 데미지, 대기, 이동은 EndState에서 관리하지 안흥ㅁ
void UBDThorFSM::BDEndState()
{
	UE_LOG(LogTemp, Warning, TEXT("End1"));

	//만약 근접 공격 상태라면
	if (mState == BDThorGeneralState::BDGiveUPFly || mState == BDThorGeneralState::BDHitDown) {
		//플레이어 근처에 있기 때문에 일단 회피 상태
		BDSetState(BDThorGeneralState::BDMove);
		
		//UE_LOG(LogTemp, Warning, TEXT("End of Attack Animation, switching to Move after delay"));
	}
	//망치를 든 근접 공격 상태였었다면
	else if (mState == BDThorGeneralState::BDHammerThreeSwing) {
		if (me->IsWeaponHold == false) {
			me->DrawWeapon(); //무기를 내린다.
		}
		BDSetState(BDThorGeneralState::BDMove);
	}
	//망치를 든 원거리 공격 상태라면
	else if (mState == BDThorGeneralState::BDHammerThrow || mState == BDThorGeneralState::BDHammerWind) {
		BDSetState(BDThorGeneralState::BDMove);
	}
	//회피를 끝냈다면
	else if (mState == BDThorGeneralState::BDAvoidance) {
		UE_LOG(LogTemp, Warning, TEXT("EndAvoidance"));
		BDSetState(BDThorGeneralState::BDMove); //원래는 공격 패턴 랜덤으로 가야함
	}
	//데미지를 받았다면
	else if (mState == BDThorGeneralState::BDDamage) {
		UE_LOG(LogTemp, Warning, TEXT("EndDamage"));

		//체력이 남아있다면
		if (BDCurrentHP > 0) {
			//상태를 회피로 전환
			BDSetState(BDThorGeneralState::BDAvoidance);
		}
		else if (BDCurrentHP <= 0) {
			//2 페이즈로 전환
		}
	}
}


//애니메이션 상태 변경
void UBDThorFSM::BDSetState(BDThorGeneralState BDnewState)
{
	mState = BDnewState; //상태 지정
	anim->animState = mState;
	//UE_LOG(LogTemp, Warning, TEXT("State changed to: %s"), *UEnum::GetValueAsString(mState));
}



