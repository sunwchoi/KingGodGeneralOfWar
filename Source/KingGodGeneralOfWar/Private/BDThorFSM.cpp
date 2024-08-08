// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorFSM.h"
#include "BDThor.h"
#include "Kratos.h"
#include "BDThorAnim.h"
#include "Kismet/GameplayStatics.h"
#include "BDThorMjolnir.h"
#include "SG_GodOfWar_GameModeBase.h"
#include "BDThorHP.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


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
		BDAvoidanceState(); //옆으로 회피
		break;
	case BDThorGeneralState::BDBackDodge:
		BDBackDodgeState(); //옆으로 회피
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
	//방향 돌리기
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

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

	//방향 돌리기
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	if (dirR.Size() >= 170.0f) {
		//플레이어와의 거리가 멀때
		me->AddMovementInput(dirR.GetSafeNormal());
	}
	else {
		//가까울 경우
		FVector dirL = myLoc - targetLoc;
		me->AddMovementInput(dirL.GetSafeNormal());
	}


	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	if (BDCurrentTime > BDDelayTime) {
		//mState = BDThorGeneralState::BDAttackModeChange;

		////애니메이션 상태 동기화
		//anim->animState = mState;

		BDSetState(BDThorGeneralState::BDAttackModeChange);
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

	bBDAttackCheck = false;

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
	else if (NewState == BDThorGeneralState::BDHammerThreeSwing) {
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

//데미지를 받을 시 발생하는 함수, 두번째는 애니메이션을 재생할 히트 방향을 말한다.
void UBDThorFSM::Damage(float DamageNum, EAttackDirectionType AtkDir)
{
	BDCurrentHP -= DamageNum;

	me->UpdateHpUI(); //체력 설정

	BDGetHitDirectionString(AtkDir);

	//피격 상태로 변경한다.
	mState = BDThorGeneralState::BDDamage;

	//UE_LOG(LogTemp, Warning, TEXT("BDThor damage!"));

}

void UBDThorFSM::BDGetHitDirectionString(EAttackDirectionType AtkDir)
{
	Str = UEnum::GetValueAsString(AtkDir).Mid(22);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Str);
}

void UBDThorFSM::BDDamageState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorDamageMontage)) {
		//UE_LOG(LogTemp, Warning, TEXT("Damage"));
		//피격 애니메이션 재생
		anim->playBDDamage();
		//점프
		anim->BDJumpToHitSection(Str);
	}

	//UE_LOG(LogTemp, Warning, TEXT("DamageState"));

}

void UBDThorFSM::BDAvoidanceState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorRightDodgeMontage)) {
		UE_LOG(LogTemp, Warning, TEXT("Avoidance")); 
		anim->playBDRightDodge(); //옆으로 회피
	}
}

void UBDThorFSM::BDBackDodgeState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorBackDodgeMontage)) {
		anim->playBDBackDodge(); //뒤로 회피
	}
}


//망치 날리면서 번개 공격
void UBDThorFSM::BDHammerThrowState()
{

	//방향 돌리기
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	// 현재 상태가 이미 Hammer Wind라면 함수 호출을 건너뛴다.
	if (mState != BDThorGeneralState::BDHammerThrow) {
		return;
	}

	//애니메이션 몽타주를 재생하고 있지 않을때, 플레이어와의 현재 거리가 250 이하일때
	if (!anim->Montage_IsPlaying(anim->BDHammerThrowMontage)) {
		anim->playBDHammerThrow();
	}


}

//망치 휘두르면서 바람공격
void UBDThorFSM::BDHammerWindState()
{
	//방향 돌리기
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

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
		//UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Wind Animation"));
	}

}

//망치 3번 휘두르기
void UBDThorFSM::BDHammerThreeSwingState()
{
	//대쉬
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	float dist = FVector::Dist(Target->GetActorLocation(), me->GetActorLocation());

	me->AddMovementInput(dirR);

	//UE_LOG(LogTemp, Warning, TEXT("bIsDash: %s"), anim->bIsDash ? TEXT("true") : TEXT("false"));
	//몽타주 실행

	anim->playBDHammerThreeSwing(); //3번 휘두르기

	//타겟과 토르의 거리가 가까울 경우
	// 타겟과 토르의 거리가 가까울 경우
	if (anim->Montage_IsPlaying(anim->BDHammerThreeSwingMontage))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hammer Three Swing Montage is playing"));

		if (dirR.Size() <= 200.0f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("State1: %s"), *UEnum::GetValueAsString(mState));
			// 몽타주 섹션을 "Attack1"으로 변경

			me->GetCharacterMovement()->MaxWalkSpeed = 100.f; // 달리는 걸 멈추고

			if (dirR.Size() <= 100.0f) {
				me->GetCharacterMovement()->MaxWalkSpeed = 50.f;
			}

			if (!bBDAttackCheck) {
				anim->Montage_JumpToSection(FName("Attack1"), anim->BDHammerThreeSwingMontage);
				bBDAttackCheck = true;
			}
			
		}
		// 타겟과 토르의 거리가 멀 경우
		else if (dirR.Size() > 200.0f)
		{
			/*if (dirR.Size() <= 100.0f) {
				me->GetCharacterMovement()->MaxWalkSpeed = 50.f;
				anim->Montage_JumpToSection(FName("Attack1"), anim->BDHammerThreeSwingMontage);
				bBDAttackCheck = true;
			}*/

			//UE_LOG(LogTemp, Warning, TEXT("State2: %s"), *UEnum::GetValueAsString(mState));
			me->GetCharacterMovement()->MaxWalkSpeed = 9000.f;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hammer Three Swing Montage is NOT playing"));
	}
}

//플레이어 잡아서 업어치기 (카메라 컷씬)
void UBDThorFSM::BDGiveUPFlyState()
{
}

//바닥을 주먹으로 내려치기
void UBDThorFSM::BDHittingDownState()
{
	//
}

void UBDThorFSM::BDDash()
{
	//방향 돌리기
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	float dist = FVector::Dist(Target->GetActorLocation(), me->GetActorLocation());

	me->AddMovementInput(dirR);

	UE_LOG(LogTemp, Warning, TEXT("BDDash"));
	//타겟과 토르의 거리가 가가울 경우
	if (dist < 200.f)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	}
	//타겟과 토르의 거리가 멀 경우
	else if (dist < 600.f)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 3000.f;
	}
	else
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 10000.f; //너무 먼 경우 빨리 오도록
		//if (me->GetCharacterMovement()->MaxWalkSpeed < 500.f)
		//	me->GetCharacterMovement()->MaxWalkSpeed += 10.f;
		//else
		//	me->GetCharacterMovement()->MaxWalkSpeed = 10000.f; //너무 먼 경우 빨리 오도록
	}

}


//애니메이션 마지막 종료 후 상태 확인 
// 대기, 이동은 EndState에서 관리하지 않는다.
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
		BDSetState(BDThorGeneralState::BDBackDodge);
	}
	//망치를 든 원거리 공격 상태라면
	else if (mState == BDThorGeneralState::BDHammerThrow || mState == BDThorGeneralState::BDHammerWind) {
		BDSetState(BDThorGeneralState::BDMove);
	}
	//옆으로 회피를 끝냈다면
	else if (mState == BDThorGeneralState::BDAvoidance) {
		//UE_LOG(LogTemp, Warning, TEXT("EndAvoidance"));
		BDSetState(BDThorGeneralState::BDAttackModeChange); //원래는 공격 패턴 랜덤으로 가야함
	}
	//뒤로 회피 했다면
	else if (mState == BDThorGeneralState::BDBackDodge) {
		BDSetState(BDThorGeneralState::BDIdle);
	}
	//데미지를 받았다면
	else if (mState == BDThorGeneralState::BDDamage) {
		UE_LOG(LogTemp, Warning, TEXT("EndDamage"));

		//체력이 남아있다면
		if (BDCurrentHP > 0) {
			//상태를 회피로 전환
			BDSetState(BDThorGeneralState::BDAvoidance);
			//BDSetState(BDThorGeneralState::BDMove);
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



