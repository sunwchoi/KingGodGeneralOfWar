// Fill out your copyright notice in the Description page of Project Settings.


#include "WindSlash.h"
#include "Components/BoxComponent.h"
#include "Kratos.h"

// Sets default values
AWindSlash::AWindSlash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	col = CreateDefaultSubobject<UBoxComponent>(TEXT("col"));
	SetRootComponent(col);
	col->SetCollisionProfileName("BDThorMjonlr");

	slashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("slashMesh"));
	slashMesh->SetupAttachment(col);
	slashMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AWindSlash::BeginPlay()
{
	Super::BeginPlay();

	col->OnComponentBeginOverlap.AddDynamic(this, &AWindSlash::OnWindOverlap);

	SetLifeSpan(4);

	// 초기 속도를 설정 (예: 1000)
	float InitialSpeed = 1500.0f;

	CurrentVelocity = GetActorForwardVector() * InitialSpeed;
	Acceleration = FVector::ZeroVector;

}

// Called every frame
void AWindSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 캐릭터의 전방 방향으로 가속도를 설정
	Acceleration = GetActorForwardVector() * 3500.0f;  // 원하는 가속도 크기 설정

	// 가속도를 이용해 속도 업데이트
	CurrentVelocity += Acceleration * DeltaTime;

	// 속도를 이용해 위치 업데이트
	SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);

	//SetActorLocation(GetActorLocation() + GetActorForwardVector() * 1500 * DeltaTime); 
}

void AWindSlash::OnWindOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Over"));

	AKratos* kratos = Cast <AKratos>(OtherActor);
	if (kratos != nullptr) {
		kratos->Damage(this, 10, EHitType::STAGGER, false); //피격 상태 임시
		this->Destroy(); //플레이어일시 데미지 주고 파괴
		//UE_LOG(LogTemp, Warning, TEXT("WindSlash"));
	}
}

