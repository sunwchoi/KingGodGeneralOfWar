// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThor.h"
#include "BDThorFSM.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABDThor::ABDThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//토르 스켈레탈 메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Bada/Asset/Model/BDThor/BDThor.BDThor'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		//메시 크기 조정
		GetMesh()->SetRelativeScale3D(FVector(0.25f));
	}

	//무기 데이터 로드
	BDWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BDWeapon"));
	BDWeapon->SetupAttachment(GetMesh(), FName("BDMjolnirHips"));
	BDWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); //임시 코드

	ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));
	if (WeaponMesh.Succeeded()) {
		BDWeapon->SetStaticMesh(WeaponMesh.Object);

	}

	//BDThorFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UBDThorFSM>(TEXT("FSM"));

	//애니메이션 블루프린트 할당하기
	ConstructorHelpers::FClassFinder<UAnimInstance> BDThorAnimation(TEXT("/Script/Engine.AnimBlueprint'/Game/Bada/BDAnimation/ABP_BDThor.ABP_BDThor'"));
	if (BDThorAnimation.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(BDThorAnimation.Class);
	}
}

// Called when the game starts or when spawned
void ABDThor::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true; //플레이어 방향으로 바라보기

	//로드할 때 무기는 일단 허리에 보이게 하기
	BDWeapon->SetVisibility(true);
	
}

// Called every frame
void ABDThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABDThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

