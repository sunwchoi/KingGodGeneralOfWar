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

	//BDThorFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UBDThorFSM>(TEXT("FSM"));

	//애니메이션 블루프린트 할당하기
	/*ConstructorHelpers::FClassFinder<UAnimInstance> BDThorAnimation(TEXT("/Script/Engine.AnimBlueprint'/Game/Bada/BDAnimation/ABP_BDThor.ABP_BDThor'"));
	if (BDThorAnimation.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(BDThorAnimation.Class);
	}*/
}

// Called when the game starts or when spawned
void ABDThor::BeginPlay()
{
	Super::BeginPlay();

	//GetCharacterMovement()->bOrientRotationToMovement = true; //플레이어 방향으로 바라보기

	//로드할 때 무기는 일단 허리에 보이게 하기
	visibleWeapon();

	//UI 보여주기
	if (BDThorHPClass) {
		UUserWidget* BDHPBar = CreateWidget<UUserWidget>(GetWorld(), BDThorHPClass);
		BDThorHPBar = Cast<UBDThorHP>(BDHPBar);
		if (BDThorHPBar) {
			BDThorHPBar->AddToViewport();
			BDThorHPBar->SetVisibility(ESlateVisibility::Visible);
		}
	}

	UpdateHpUI();
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
		UE_LOG(LogTemp, Warning, TEXT("Equip Weapon"));
	}
}


//무기 위치를 허리로 변경하는 함수
void ABDThor::DrawWeapon()
{
	if (this) {
		IsWeaponHold = false;
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirHips"));
		BDWeapon->SetVisibility(true);
	}
}

//오른손에 들기
void ABDThor::EquipRight()
{
	if (this) {
		IsWeaponHold = true;
		BDWeapon->SetVisibility(true);
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirRightHand"));
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
		UE_LOG(LogTemp, Warning, TEXT("Visible"));
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
		UE_LOG(LogTemp, Warning, TEXT("Mjolnir spawned successfully and fired"));
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

void ABDThor::UpdateHpUI()
{
	if (BDThorHPBar) {
		BDThorHPBar->SetHP(fsm->BDCurrentHP, fsm->BDMaxHp);
	}

	if (fsm->BDCurrentHP <= 0) {
		//2 페이즈로 전환
		ASG_GodOfWar_GameModeBase* BDGameMode = Cast<ASG_GodOfWar_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); //게임모드 캐스트

		if (BDThorHPBar) {
			BDThorHPBar->RemoveFromParent(); //HP가 0이 되면 삭제해라
		}

		if (BDGameMode) {
			BDGameMode->ThorFadeOut(); //페이드 아웃 실행
		}


	}

}

