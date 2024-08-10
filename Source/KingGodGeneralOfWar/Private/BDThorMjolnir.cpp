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

		MjolnirMesh->SetRelativeLocation(FVector(0, 0, -50.0f));

	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(MjoCol);

	//스피드 설정
	MovementComp->InitialSpeed = 2500.0f; //원래는 1500
	MovementComp->MaxSpeed = 2500.0f;
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

	if (Thor) {
		UE_LOG(LogTemp, Warning, TEXT("Thor found in BeginPlay"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Thor is null in BeginPlay"));
	}

	bReturning = false;
	bCreateTrue = false;
}

// Called every frame
void ABDThorMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//날아가는 중
	if (!bReturning && bCreateTrue) {
		MovementComp->Velocity = Thor->GetActorForwardVector() * MovementComp->InitialSpeed;
	}
	//망치 돌아옴
	else if (bReturning && bCreateTrue)
	{
		FVector HandLocation = Thor->GetMesh()->GetSocketLocation(TEXT("BDMjolnirHand"));
		FVector CurrentLocation = GetActorLocation();
		FVector Direction = (HandLocation - CurrentLocation).GetSafeNormal();

		//SetActorLocation(GetActorLocation() + Direction * Speed * GetWorld()->DeltaTimeSeconds);

		//MovementComp->Velocity = Direction * MovementComp->InitialSpeed;

		//거리 계산
		float Distance = FVector::Dist(GetActorLocation(), HandLocation);

		// 최소 거리와 최대 거리 설정
		float MinDistance = 200.0f;
		float MaxDistance = 2000.0f;

		//토르에게 올수록 느려짐
		// 거리 비율 계산 (0.0f에서 1.0f 사이의 값)
		//float DistanceRatio = FMath::Clamp((Distance - MinDistance) / (MaxDistance - MinDistance), 0.0f, 1.0f);

		//// 속도를 거리에 따라 조절 (거리 비율이 0에 가까울수록 속도가 느려짐)
		//float AdjustedSpeed = FMath::Lerp(500.0f, MovementComp->InitialSpeed, DistanceRatio);

		//토르에게 올수록 빨라짐
		// 거리 비율 계산 (0.0f에서 1.0f 사이의 값)
		float DistanceRatio = FMath::Clamp((Distance - MinDistance) / (MaxDistance - MinDistance), 0.0f, 1.0f);

		//// 속도를 거리에 따라 조절 (거리 비율이 1에 가까울수록 속도가 빨라짐)
		float AdjustedSpeed = FMath::Lerp(MovementComp->InitialSpeed, 4000.0f, 1.0f - DistanceRatio);

		MovementComp->Velocity = Direction * AdjustedSpeed;

		if (Distance < 170.0f) // 거리 체크
		{
			bReturning = false;
			bCreateTrue = false;
			this->Destroy(); // 자신을 파괴
			UE_LOG(LogTemp, Warning, TEXT("Destroy"));
			Thor->EquipWeapon(); // 무기 다시 보이게 하기
			UE_LOG(LogTemp, Warning, TEXT("Mjolnir Back"));

		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("bReturning: %s, bCreateTrue : %s "), bReturning ? TEXT("true") : TEXT("false"), bCreateTrue ? TEXT("true") : TEXT("false"));
}

void ABDThorMjolnir::FireInDirection(const FVector& ShootDirection)
{
	//묠니르가 전혀 스폰되지 않은 상태일때만
	if (!bCreateTrue) { 
		// MjoCol과 MjolnirMesh를 -90도로 회전시킴
		FRotator NewRotation = FRotator(90, -90, 90);
		MjoCol->SetRelativeRotation(NewRotation);
		MjolnirMesh->SetRelativeRotation(NewRotation);

		Thor->HiddenWeapon();
		bCreateTrue = true;
	}

}

//묠니르 발사 후 다시 손에 돌아오는 함수
void ABDThorMjolnir::BackMjolnir()
{
	UE_LOG(LogTemp, Warning, TEXT("BackMjolnir called"));
	bReturning = true;
	UE_LOG(LogTemp, Warning, TEXT("bReturing : %s"), bReturning ? TEXT("true") : TEXT("false"));
}

//묠니르가 부딪혔을 때 나오는 함수
void ABDThorMjolnir::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mjolnir BeginOverlap"));
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//데미지 처리
		auto* AttackTarget = Cast<AKratos>(OtherActor); //타겟일때
		if (AttackTarget) {
			AttackTarget->Damage(this, 10, EHitType::NB_HIGH, false);
			// 플레이어에게 맞을시 콜리전 비활성화
			MjoCol->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
			//UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!"));
		}
	}
}

