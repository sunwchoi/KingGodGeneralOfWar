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

	LastAttackState = BDThorGeneralState::BDIdle; // �ʱ�ȭ
}


// Called when the game starts
void UBDThorFSM::BeginPlay()
{
	Super::BeginPlay();

	//���忡�� �÷��̾� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	//AKratos Ÿ������ ĳ����
	Target = Cast<AKratos>(actor);
	//���� ��ü ��������
	me = Cast<ABDThor>(GetOwner());

	//�ִϸ��̼� �Ҵ�
	anim = Cast<UBDThorAnim>(me->GetMesh()->GetAnimInstance());

	//ü�� ����
	BDCurrentHP = BDMaxHp;

}


// Called every frame
void UBDThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState) {
	case BDThorGeneralState::BDIdle:
		BDIdleState(); //���
		break;
	case BDThorGeneralState::BDMove:
		BDMoveState(); //������
		break;
	case BDThorGeneralState::BDAttackModeChange:
		BDAttackModeChangeState(); //���� ���� ���� �Լ�
		break;
	case BDThorGeneralState::BDAvoidance:
		BDAvoidanceState(); //ȸ��
		break;
	case BDThorGeneralState::BDDamage:
		BDDamageState(); //�ǰ�
		break;
	case BDThorGeneralState::BDHammerThrow:
		BDHammerThrowState(); //���� ����
		break;
	case BDThorGeneralState::BDHammerWind:
		BDHammerWindState(); //���� ����
		break;
	case BDThorGeneralState::BDHammerThreeSwing:
		BDHammerThreeSwingState(); //���� ����
		break;
	case BDThorGeneralState::BDGiveUPFly:
		BDGiveUPFlyState(); //���� ����
		break;
	case BDThorGeneralState::BDHitDown:
		BDHittingDownState(); //���� ����
		break;
	}

	//����â�� ���� �޼��� ���
	FString logMessege = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMessege);
}


//���� ���� �Լ�
void UBDThorFSM::BDIdleState()
{
	//���� ������
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	//�ð��� �帣�鼭
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (BDCurrentTime > BDidleDelayTime) {
		//3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = BDThorGeneralState::BDMove;
		//��� �ð� �ʱ�ȭ
		BDCurrentTime = 0;

		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
}

void UBDThorFSM::BDMoveState()
{
	//�⺻�����δ� �÷��̾ ���� �̵�������
	//�ǰ� ���°� �Ǿ��ų� ������ �ϱ� ���� �÷��̾ �ٶ󺸸� �ڷ� �̵��ϰų� ������ ȸ�Ǹ� �Ѵ�.

	//���� ������
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	//�⺻ �̵� ����
	//1. Ÿ�� �������� �ʿ��ϴ�.
	FVector destination = Target->GetActorLocation();
	//2. ������ �ʿ��ϴ�.
	FVector dir = destination - me->GetActorLocation();
	//3. �������� �̵��ϰ� �ʹ�.
	me->AddMovementInput(dir.GetSafeNormal());


	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	if (BDCurrentTime > BDDelayTime) {
		//mState = BDThorGeneralState::BDAttackModeChange;

		////�ִϸ��̼� ���� ����ȭ
		//anim->animState = mState;

		BDSetState(BDThorGeneralState::BDAttackModeChange);
		BDCurrentTime = 0;
	}

	//Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	//1. ���� �Ÿ��� ���� ���� �ȿ� ������
	//if (dir.Size() < BDAttackRange) {
	//	//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
	//	//���� �� ��üȭ
	//	mState = BDThorGeneralState::BDAttackModeChange;

	//	//�ִϸ��̼� ���� ����ȭ
	//	anim->animState = mState;
	//}
}


void UBDThorFSM::BDAttackModeChangeState()
{
	//�����ð��� �پ��� �������� �Ѿ���Ѵ�.

	//�켱�� �ִϸ��̼��� ��� or ������ or ȸ�� �ִϸ��̼��� ����Ѵ�.
	//�켱�� ��� ���� �ִϸ��̼� ������� ����
	anim->playBDAttackRandomState();

	//1. �ð��� �帣��
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//2. ���� �ð��� �Ǹ�
	if (BDCurrentTime > BDAttackDelayTime) {
		// �������� ���� ���� ����
		mState = RandomAttackState();
		//mState = BDThorGeneralState::BDHammerWind;  // �ӽ� ����
		anim->animState = mState;
		
		//UE_LOG(LogTemp, Warning, TEXT("AttackModeChangeState: %s"), *UEnum::GetValueAsString(mState));
		
		BDCurrentTime = 0;
	}

	//Ÿ���� ������ ������ �ٽ� �̵����� ���ϰ� �ʹ�.

	////�ӽ� �ڵ�, �÷��̾��� Damage�� �߻����� ��� ���ʹ��� ���¸� ��� �Ǵ� �̵����� ����
	//float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	////Ÿ���� �Ÿ��� ���� ������ ������ϱ�
	//if (distance > BDAttackRange) {
	//	//���¸� ��� ���� ��ȯ�Ѵ�.
	//	mState = BDThorGeneralState::BDIdle;
	//	//�ִϸ��̼� ���� ����ȭ
	//	anim->animState = mState;
	//}

}

//���� ������ �������� �����ϴ� �Լ�
BDThorGeneralState UBDThorFSM::RandomAttackState()
{
	//�ִϸ��̼� ���´� �켱 ��Ÿ�ָ� �����Ѵ�
	
	bBDAttackCheck = false;

	// ������ ���µ��� �迭�� ����
	TArray<BDThorGeneralState> AttackStates = {
		BDThorGeneralState::BDHammerThrow,
		BDThorGeneralState::BDHammerWind,
		BDThorGeneralState::BDHammerThreeSwing,
		//BDThorGeneralState::BDGiveUPFly,
		//BDThorGeneralState::BDHitDown
	};

	// ������ ���� ����
	AttackStates.Remove(LastAttackState);

	// �������� ����
	int32 RandomIndex = FMath::RandRange(0, AttackStates.Num() - 1);
	BDThorGeneralState NewState = AttackStates[RandomIndex];

	//���� ��ġ ������ ���
	if (NewState == BDThorGeneralState::BDHammerThrow || NewState == BDThorGeneralState::BDHammerWind) {
		//�տ� ��ġ�� ����
		me->EquipWeapon();
	}
	else if(NewState == BDThorGeneralState::BDHammerThreeSwing) {
		me->EquipRight(); //�����տ� ��ġ�� ����
	}
	else {
		me->DrawWeapon(); //�㸮�� ��ġ�� �ξ��
	}

	// ������ ���� ������Ʈ
	LastAttackState = NewState;
	//UE_LOG(LogTemp, Warning, TEXT("Random!!"));
	return NewState; //���� ����
}

//�������� ���� �� �߻��ϴ� �Լ�, �ι�°�� �ִϸ��̼��� ����� ��Ʈ ������ ���Ѵ�.
void UBDThorFSM::Damage(float DamageNum, EAttackDirectionType AtkDir)
{
	BDCurrentHP -= DamageNum;

	me->UpdateHpUI(); //ü�� ����

	BDGetHitDirectionString(AtkDir);

	//�ǰ� ���·� �����Ѵ�.
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
		//�ǰ� �ִϸ��̼� ���
		anim->playBDDamage();
		//����
		anim->BDJumpToHitSection(Str);
	}

	//UE_LOG(LogTemp, Warning, TEXT("DamageState"));
	
}

void UBDThorFSM::BDAvoidanceState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorRightDodgeMontage)) {
		//�������� �޾��� �� ȸ��
		UE_LOG(LogTemp, Warning, TEXT("Avoidance"));
		anim->playBDRightDodge();
	}

	//���� ��ó ��ü ��ġ�� �޾Ƽ� ���������� ȸ���� ������ �������� ȸ���� �������� ���� �ڵ� �ۼ�

	//�÷��̾��� ������ �������� �ڷ� ���ϰų� ������ ���Ѵ�.
}


//��ġ �����鼭 ���� ����
void UBDThorFSM::BDHammerThrowState()
{

	//���� ������
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	// ���� ���°� �̹� Hammer Wind��� �Լ� ȣ���� �ǳʶڴ�.
	if (mState != BDThorGeneralState::BDHammerThrow) {
		return;
	}

	//�ִϸ��̼� ��Ÿ�ָ� ����ϰ� ���� ������, �÷��̾���� ���� �Ÿ��� 250 �����϶�
	if (!anim->Montage_IsPlaying(anim->BDHammerThrowMontage)) {
		anim->playBDHammerThrow();
	}

	
}

//��ġ �ֵθ��鼭 �ٶ�����
void UBDThorFSM::BDHammerWindState()
{
	//���� ������
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	if (me->IsWeaponHold == false) {
		me->EquipWeapon(); //�켱 ���⸦ ���.
	}

	// ���� ���°� �̹� Hammer Wind��� �Լ� ȣ���� �ǳʶڴ�.
	if (mState != BDThorGeneralState::BDHammerWind) {
		return;
	}

	// �ִϸ��̼��� ����ǰ� �ִ��� Ȯ���ϰ�, ������� �ʾҴٸ� ����Ѵ�.
	if (!anim->Montage_IsPlaying(anim->BDHammerWindMontage))
	{
		anim->playBDHammerWind();
		//UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Wind Animation"));
	}

}

//��ġ 3�� �ֵθ���
void UBDThorFSM::BDHammerThreeSwingState()
{
	//�뽬
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	float dist = FVector::Dist(Target->GetActorLocation(), me->GetActorLocation());

	me->AddMovementInput(dirR);

	//UE_LOG(LogTemp, Warning, TEXT("bIsDash: %s"), anim->bIsDash ? TEXT("true") : TEXT("false"));
	//��Ÿ�� ����

	anim->playBDHammerThreeSwing(); //3�� �ֵθ���

	//Ÿ�ٰ� �丣�� �Ÿ��� ����� ���
	// Ÿ�ٰ� �丣�� �Ÿ��� ����� ���
	if (anim->Montage_IsPlaying(anim->BDHammerThreeSwingMontage))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hammer Three Swing Montage is playing"));

		if (dirR.Size() <= 200.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("State1: %s"), *UEnum::GetValueAsString(mState));
			// ��Ÿ�� ������ "Attack1"���� ����

			me->GetCharacterMovement()->MaxWalkSpeed = 200.f; // �޸��� �� ���߰�

			if (!bBDAttackCheck) {
				anim->Montage_JumpToSection(FName("Attack1"), anim->BDHammerThreeSwingMontage);
				bBDAttackCheck = true;
			}
			//else {
			//	//���� �÷��̾ �¾Ҵٸ�
			//	//BDSetState(BDThorGeneralState::BDIdle); //���߿� �ڷ� ȸ�� �Ǵ� ������ ȸ�Ƿ� ����
			//	//bBDAttackCheck = false; //����
			//}

			//UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Three Swing Animation"));
		}
		// Ÿ�ٰ� �丣�� �Ÿ��� �� ���
		else if (dirR.Size() > 200.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("State2: %s"), *UEnum::GetValueAsString(mState));
			me->GetCharacterMovement()->MaxWalkSpeed = 10000.f;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hammer Three Swing Montage is NOT playing"));
	}
}

//�÷��̾� ��Ƽ� ����ġ�� (ī�޶� �ƾ�)
void UBDThorFSM::BDGiveUPFlyState()
{
}

//�ٴ��� �ָ����� ����ġ��
void UBDThorFSM::BDHittingDownState()
{
}

void UBDThorFSM::BDDash()
{
	//���� ������
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me->SetActorRotation(FRotator(0, rot.Yaw, 0));

	float dist = FVector::Dist(Target->GetActorLocation(), me->GetActorLocation());

	me->AddMovementInput(dirR);

	UE_LOG(LogTemp, Warning, TEXT("BDDash"));
	//Ÿ�ٰ� �丣�� �Ÿ��� ������ ���
	if (dist < 200.f)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	}
	//Ÿ�ٰ� �丣�� �Ÿ��� �� ���
	else if (dist < 600.f)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 3000.f;
	}
	else
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 10000.f; //�ʹ� �� ��� ���� ������
		//if (me->GetCharacterMovement()->MaxWalkSpeed < 500.f)
		//	me->GetCharacterMovement()->MaxWalkSpeed += 10.f;
		//else
		//	me->GetCharacterMovement()->MaxWalkSpeed = 10000.f; //�ʹ� �� ��� ���� ������
	}

}


//�ִϸ��̼� ������ ���� �� ���� Ȯ�� 
// ���, �̵��� EndState���� �������� �ʴ´�.
void UBDThorFSM::BDEndState()
{
	UE_LOG(LogTemp, Warning, TEXT("End1"));

	//���� ���� ���� ���¶��
	if (mState == BDThorGeneralState::BDGiveUPFly || mState == BDThorGeneralState::BDHitDown) {
		//�÷��̾� ��ó�� �ֱ� ������ �ϴ� ȸ�� ����
		BDSetState(BDThorGeneralState::BDMove);
		
		//UE_LOG(LogTemp, Warning, TEXT("End of Attack Animation, switching to Move after delay"));
	}
	//��ġ�� �� ���� ���� ���¿����ٸ�
	else if (mState == BDThorGeneralState::BDHammerThreeSwing) {
		if (me->IsWeaponHold == false) {
			me->DrawWeapon(); //���⸦ ������.
		}
		//BDSetState(BDThorGeneralState::BDIdle);
	}
	//��ġ�� �� ���Ÿ� ���� ���¶��
	else if (mState == BDThorGeneralState::BDHammerThrow || mState == BDThorGeneralState::BDHammerWind) {
		BDSetState(BDThorGeneralState::BDMove);
	}
	//ȸ�Ǹ� ���´ٸ�
	else if (mState == BDThorGeneralState::BDAvoidance) {
		UE_LOG(LogTemp, Warning, TEXT("EndAvoidance"));
		BDSetState(BDThorGeneralState::BDMove); //������ ���� ���� �������� ������
	}
	//�������� �޾Ҵٸ�
	else if (mState == BDThorGeneralState::BDDamage) {
		UE_LOG(LogTemp, Warning, TEXT("EndDamage"));

		//ü���� �����ִٸ�
		if (BDCurrentHP > 0) {
			//���¸� ȸ�Ƿ� ��ȯ
			//BDSetState(BDThorGeneralState::BDAvoidance);
			BDSetState(BDThorGeneralState::BDMove);
		}
	}
}


//�ִϸ��̼� ���� ����
void UBDThorFSM::BDSetState(BDThorGeneralState BDnewState)
{
	mState = BDnewState; //���� ����
	anim->animState = mState;
	//UE_LOG(LogTemp, Warning, TEXT("State changed to: %s"), *UEnum::GetValueAsString(mState));
}



