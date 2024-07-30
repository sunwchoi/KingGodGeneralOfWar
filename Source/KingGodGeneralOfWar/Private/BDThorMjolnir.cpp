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
	MjoCol->SetCollisionProfileName(FName("BDThorMjonlr"));

	//메쉬설정
	MjolnirMesh = CreateDefaultSubobject<UStaticMeshComponent>("MjolnirMesh");
	MjolnirMesh->SetupAttachment(RootComponent);
	//메쉬 부착
	ConstructorHelpers::FObjectFinder <UStaticMesh> MeshComp(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));

	if (MeshComp.Succeeded()) {
		MjolnirMesh->SetStaticMesh(MeshComp.Object);
		MjolnirMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		MjolnirMesh->SetRelativeLocation(FVector(0, 0, -30.0f));

	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(MjoCol);

	//스피드 설정
	MovementComp->InitialSpeed = 0.0f; //원래는 1500
	MovementComp->MaxSpeed = 0.0f;
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
	
	//OnOverlap 이벤트 연결
	MjoCol->OnComponentBeginOverlap.AddDynamic(this, &ABDThorMjolnir::OnOverlap);
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

	// MjoCol과 MjolnirMesh를 -90도로 회전시킴
	FRotator NewRotation = FRotator(0, -90, 90);
	MjoCol->SetRelativeRotation(NewRotation);
	MjolnirMesh->SetRelativeRotation(NewRotation);

	MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;

}

//묠니르 발사 후 다시 손에 돌아오는 함수
void ABDThorMjolnir::BackMjolnir()
{
}

//묠니르가 부딪혔을 때 나오는 함수
void ABDThorMjolnir::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Mjolnir BeginOverlap"));

	if (OtherActor && (OtherActor != this) && OtherComp) {
		//데미지 처리
		auto* AttackTarget = Cast<AKratos>(OtherActor); //타겟일때
		if (AttackTarget) {
			//공격 상태 나누기
			AttackTarget->Damage(BDThrowDamage, EAttackType::Attack1);
			UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!"));
		}
	}
}

