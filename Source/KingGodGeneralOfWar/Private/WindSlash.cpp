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

	SetLifeSpan(5);
}

// Called every frame
void AWindSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * 2500 * DeltaTime);
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

