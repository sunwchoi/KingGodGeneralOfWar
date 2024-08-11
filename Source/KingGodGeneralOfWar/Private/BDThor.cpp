// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThor.h"
#include "BDThorFSM.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kratos.h"
#include "WindSlash.h"
#include "BDThorMjolnir.h"
#include "BDThorHP.h"
#include "Blueprint/UserWidget.h"
#include "SG_GodOfWar_GameModeBase.h"
#include "DrawDebugHelpers.h" //디버그 확인용
#include "Components/CapsuleComponent.h"
#include "SG_Shield.h"
#include "Components/SphereComponent.h"
#include "CSW/CSWGameMode.h"

// Sets default values
ABDThor::ABDThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//토르 스켈레탈 메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Bada/Asset/Model/BDThor/BDThor.BDThor'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//메시 크기 조정
		GetMesh()->SetRelativeScale3D(FVector(0.25f));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -118.0f)); //메쉬 위치 조정
	}

	//무기 데이터 로드
	BDWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BDWeapon"));
	BDWeapon->SetupAttachment(GetMesh(), FName("BDMjolnirHips"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));
	if (WeaponMesh.Succeeded()) {
		BDWeapon->SetStaticMesh(WeaponMesh.Object);
		BDWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); //임시 코드
	}

	//콜리전 설정
	BDWeaponCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BDWeaponCol"));
	BDWeaponCol->SetupAttachment(BDWeapon); //무기에 콜리전 붙이기
	BDWeaponCol->SetCollisionProfileName(FName("BDThorMjonlr"));

	//BDThorFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UBDThorFSM>(TEXT("FSM"));

	//애니메이션 블루프린트 할당하기
	/*ConstructorHelpers::FClassFinder<UAnimInstance> BDThorAnimation(TEXT("/Script/Engine.AnimBlueprint'/Game/Bada/BDAnimation/ABP_BDThor.ABP_BDThor_C'"));
	if (BDThorAnimation.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(BDThorAnimation.Class);
	}*/

}

// Called when the game starts or when spawned
void ABDThor::BeginPlay()
{
	Super::BeginPlay();

	//ThorAreaSpere->SetCollisionEnabled(ECollisionEnabled::NoCollision); //처음에 켜질땐 비활성화, 영역함수가 불릴때만 true

	BDWeaponCol->OnComponentBeginOverlap.AddDynamic(this, &ABDThor::BDWeaponOverlap);

	//ThorAreaSpere->OnComponentBeginOverlap.AddDynamic(this, &ABDThor::BDSphereOverlap);

	//로드할 때 무기는 일단 허리에 보이게 하기
	visibleWeapon();

	////UI 보여주기
	//if (BDThorHPClass) {
	//	UUserWidget* BDHPBar = CreateWidget<UUserWidget>(GetWorld(), BDThorHPClass);
	//	BDThorHPBar = Cast<UBDThorHP>(BDHPBar);
	//	if (BDThorHPBar) {
	//		BDThorHPBar->AddToViewport();
	//		BDThorHPBar->SetVisibility(ESlateVisibility::Visible);
	//	}
	//}

	fsm->BDCurrentHP = fsm->BDMaxHp; //체력 설정

	GameMode = Cast<ACSWGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->SetEnemyHpBar(fsm->BDCurrentHP / fsm->BDMaxHp);
	//UpdateHpUI();
}

// Called every frame
void ABDThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called to bind functionality to input
void ABDThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//무기 위치를 손으로 변경하는 함수
void ABDThor::EquipWeapon()
{
	if (this) {
		IsWeaponHold = true;
		BDWeapon->SetVisibility(true);
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirHand"));
		BDWeaponCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//UE_LOG(LogTemp, Warning, TEXT("Equip Weapon"));
	}
}


//무기 위치를 허리로 변경하는 함수
void ABDThor::DrawWeapon()
{
	if (this) {
		IsWeaponHold = false;
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirHips"));
		BDWeapon->SetVisibility(true);
		BDWeaponCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//오른손에 들기
void ABDThor::EquipRight()
{
	if (this) {
		IsWeaponHold = true;
		BDWeapon->SetVisibility(true);
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirRightHand"));
		BDWeaponCol->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ABDThor::HiddenWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("Hidden"));
	if (this) {
		IsWeaponHold = false;
		BDWeapon->SetVisibility(false); //메쉬 안보이게 하기
	}
}

void ABDThor::visibleWeapon()
{
	if (this) {
		IsWeaponHold = true;
		BDWeapon->SetVisibility(true); //메쉬 안보이게 하기
		//UE_LOG(LogTemp, Warning, TEXT("Visible"));
	}
}

void ABDThor::BDHammerThrowHit()
{
	FTransform t = GetMesh()->GetSocketTransform(TEXT("BDMjolnirHand"));
	//ABDThorMjolnir* Mjolnir = GetWorld()->SpawnActor<ABDThorMjolnir>(MjolnirFactory, t);
	FlyWeapon  = GetWorld()->SpawnActor<ABDThorMjolnir>(MjolnirFactory, t);

	if (FlyWeapon)
	{
		//FVector LaunchDirection = (fsm->Target->GetActorLocation() - t.GetLocation()).GetSafeNormal();
		FVector LaunchDirection = GetActorForwardVector();
		FlyWeapon->FireInDirection(LaunchDirection);
		//UE_LOG(LogTemp, Warning, TEXT("Mjolnir spawned successfully and fired"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Mjolnir spawned failed"));
	}
}

//바람 슬래쉬
void ABDThor::BDHammerWindSlash()
{
	//바람 액터 스폰해서 공격
	FActorSpawnParameters parm;
	parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AWindSlash>(SlashFat, GetActorLocation(), GetActorRotation(), parm);
}

//void ABDThor::UpdateHpUI()
//{
//	if (BDThorHPBar) {
//		BDThorHPBar->SetHP(fsm->BDCurrentHP, fsm->BDMaxHp);
//	}
//
//	if (fsm->BDCurrentHP < 0) {
//		//2 페이즈로 전환
//		ASG_GodOfWar_GameModeBase* BDGameMode = Cast<ASG_GodOfWar_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); //게임모드 캐스트
//
//		if (BDThorHPBar) {
//			BDThorHPBar->RemoveFromParent(); //HP가 0이 되면 삭제해라
//		}
//
//		if (BDGameMode) {
//			BDGameMode->ThorFadeOut(); //페이드 아웃 실행
//		}
//
//
//	}
//
//}

void ABDThor::BDWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//데미지 처리
		auto* AttackTarget = Cast<AKratos>(OtherActor); //타겟일때
		auto* shield = Cast<ASG_Shield>(OtherActor);
		if (AttackTarget) {
			fsm->bBDAttackCheck = true;
			AttackTarget->Damage(this, 10, EHitType::NB_HIGH, false);
			fsm->BDSetState(BDThorGeneralState::BDBackDodge);
			//fsm->BDSetState(BDThorGeneralState::BDAvoidance);
			UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!")); //회피로 변경
		}
		else if(shield){
			UE_LOG(LogTemp, Warning, TEXT("shield Attack"));
		}
	}
}

//영역에 플레이어가 잇을 경우 공격 데미지 들어감
//void ABDThor::BDSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor && (OtherActor != this) && OtherComp) {
//		//데미지 처리
//		auto* AttackTarget = Cast<AKratos>(OtherActor); //타겟일때
//		if (AttackTarget) {
//			fsm->bBDAttackCheck = true;
//			AttackTarget->Damage(this, 10, EHitType::NB_HIGH, false);
//			fsm->BDSetState(BDThorGeneralState::BDIdle); //대기로 변경
//			//UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!")); //회피로 변경
//		}
//	}
//}



