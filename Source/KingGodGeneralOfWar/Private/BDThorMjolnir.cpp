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
	MovementComp->InitialSpeed = 2000.0f; //원래는 1500
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = false;
	MovementComp->ProjectileGravityScale = 0.0f; 

	//월드에서 플레이어 찾아오기
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	////AKratos 타입으로 캐스팅
	//Target = Cast<AKratos>(actor);
	////소유 객체 가져오기
	
	//me = Cast<ABDThor>(GetOwner());
	Thor = nullptr; // null로 초기화하고 BeginPlay에서 설정

}

// Called when the game starts or when spawned
void ABDThorMjolnir::BeginPlay()
{
	Super::BeginPlay();
	
	//OnOverlap 이벤트 연결
	MjoCol->OnComponentBeginOverlap.AddDynamic(this, &ABDThorMjolnir::OnOverlap);

	// 소유 객체 가져오기
	//me = Cast<ABDThor>(GetOwner());
	Thor = Cast<ABDThor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABDThor::StaticClass()));
	if ( Thor )
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner set successfully"));

		// 테스트 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(Timerhandle, this, &ABDThorMjolnir::BackMjolnir, 5.0f, false);
		//UE_LOG(LogTemp, Warning, TEXT("Test timer set for BackMjolnir in BeginPlay"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is null"));
	}
}

// Called every frame
void ABDThorMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABDThorMjolnir::FireInDirection(const FVector& ShootDirection)
{

	// MjoCol과 MjolnirMesh를 -90도로 회전시킴
	FRotator NewRotation = FRotator(0, -90, 90);
	MjoCol->SetRelativeRotation(NewRotation);
	MjolnirMesh->SetRelativeRotation(NewRotation);

	MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;

	Thor->HiddenWeapon();

	if (GetWorld())
	{
		//UE_LOG(LogTemp, Warning, TEXT("GetWorld() is valid"));

		//// 월드와 타이머 관리자 유효성 확인
		//if (GetWorld()->GetTimerManager().IsTimerActive(Timerhandle))
		//{
		//	//UE_LOG(LogTemp, Warning, TEXT("Clearing existing timer"));
		//	GetWorld()->GetTimerManager().ClearTimer(Timerhandle);
		//}

		GetWorld()->GetTimerManager().SetTimer(Timerhandle, this, &ABDThorMjolnir::BackMjolnir, 1.0f, false);
		//UE_LOG(LogTemp, Warning, TEXT("Timer set for BackMjolnir"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is null"));
	}

	// 일정 시간이 지나면 BackMjolnir 함수를 호출하도록 타이머 설정
	//GetWorld()->GetTimerManager().SetTimer(Timerhandle, this, &ABDThorMjolnir::BackMjolnir, 3.0f, false);

}

//묠니르 발사 후 다시 손에 돌아오는 함수
void ABDThorMjolnir::BackMjolnir()
{
	//UE_LOG(LogTemp, Warning, TEXT("BackMjolnir called"));
	if (Thor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Returning Mjolnir"));
		FVector Direction = (Thor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		MovementComp->Velocity = Direction * MovementComp->InitialSpeed;

		FTimerHandle tmp;
		GetWorld()->GetTimerManager().SetTimer(tmp, [&, this]() {
			FVector Direction = (Thor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			MovementComp->Velocity = Direction * MovementComp->InitialSpeed;
			//묠니르의 거리가 가까워질때 
			FVector DistanceVector = Thor->GetActorLocation() - GetActorLocation();
			float Distance = DistanceVector.Size();

			if (Distance < 50.0f) // 거리 체크
			{
				if (IsFly) {
					this->Destroy(); // 자신을 파괴
					GetWorld()->GetTimerManager().ClearTimer(tmp); // 타이머 멈추기
					UE_LOG(LogTemp, Warning, TEXT("Destory"));
					Thor->visibleWeapon(); // 무기 다시 보이게 하기
				}
			}

			}, 0.1f, true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Owner (me) is null in BackMjolnir"));
	}
}

void ABDThorMjolnir::SetIsFly()
{
	IsFly = true;
}

//묠니르가 부딪혔을 때 나오는 함수
void ABDThorMjolnir::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mjolnir BeginOverlap"));
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//데미지 처리
		auto* AttackTarget = Cast<AKratos>(OtherActor); //타겟일때
		if (AttackTarget) {
			//공격 상태 나누기
			//여기에 데미지랑 공격 상태 부르는 함수들 만들어서 타입이랑 공격 상태 나누기
			//AttackTarget->Damage(BDThrowDamage, EAttackType::Attack1);
			//UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!"));
		}
	}
}

