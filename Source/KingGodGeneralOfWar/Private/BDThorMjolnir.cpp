// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorMjolnir.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kratos.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABDThorMjolnir::ABDThorMjolnir()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌 설정
	MjoCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MjoCol"));
	SetRootComponent(MjoCol);

	//메쉬설정
	MjolnirMesh = CreateDefaultSubobject<UStaticMeshComponent>("MjolnirMesh");
	MjolnirMesh->SetupAttachment(RootComponent);
	//메쉬 부착
	ConstructorHelpers::FObjectFinder <UStaticMesh> MeshComp(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));

	if (MeshComp.Succeeded()) {
		MjolnirMesh->SetStaticMesh(MeshComp.Object);
		MjolnirMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		MjolnirMesh->SetRelativeLocation(FVector(0, 0, -30.0f));

		//망치를 던질 때 사용할 것이기 때문에 망치의 윗부분을 바라보아야 함, 회전 설정 필요

	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(MjoCol);

	//스피드 설정
	MovementComp->InitialSpeed = 1500.0f;
	MovementComp->MaxSpeed = 1500.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = false;
	MovementComp->ProjectileGravityScale = 0.0f; 

	//월드에서 플레이어 찾아오기
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	////AKratos 타입으로 캐스팅
	//Target = Cast<AKratos>(actor);
	////소유 객체 가져오기
	//me = Cast<ABDThor>(GetOwner());

}

// Called when the game starts or when spawned
void ABDThorMjolnir::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABDThorMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 여기 수정
	/*float Speed = 1000.0f;
	FVector Dir = GetActorForwardVector();
	FVector velo = Dir * Speed;
	SetActorLocation(GetActorLocation() + velo * DeltaTime);*/

	//float Speed = 500.0f;
	////플레이어를 향해서 날라가게 하기
	//FVector dir = Target->GetActorLocation() - me->GetActorLocation();
	//dir.Normalize();
	//FVector velo = dir * Speed;
	//SetActorLocation(GetActorLocation() + velo * DeltaTime);

	// 2초 후 자동 삭제
	//SetLifeSpan(2.0f);
}

void ABDThorMjolnir::FireInDirection(const FVector& ShootDirection)
{
	MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;

}

