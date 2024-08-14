// Fill out your copyright notice in the Description page of Project Settings.


#include "RuneAttackField.h"
#include "Components/BoxComponent.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorFSM.h"
// Sets default values
int8 MaxHitCnt = 4;
ARuneAttackField::ARuneAttackField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);

}

// Called when the game starts or when spawned
void ARuneAttackField::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ARuneAttackField::OnFieldOverlap);
	FTimerHandle destroyHandle;

	GetWorld()->GetTimerManager().SetTimer(destroyHandle, [&]()
		{
			this->Destroy();
		}, 3.5f, false);
}

// Called every frame
void ARuneAttackField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARuneAttackField::OnFieldOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitCnt >= MaxHitCnt) return;
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	auto* Thor = Cast<ABDThor>(OtherActor);
	const float fieldDamage = 6;
	const float fieldDalay = 0.25;

	if (Thor)
	{
		Thor->fsm->Damage(fieldDamage, EAttackDirectionType::UP);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
			{
				boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			}, fieldDalay, false);
			HitCnt++;
	}
	else
	{
		auto AwakenThor = Cast<AAwakenThor>(OtherActor);
		AwakenThor->getFSM()->SetDamage(fieldDamage, EAttackDirectionType::UP, true);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
			{
				boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			}, fieldDalay, false);
		HitCnt++;
	}

}

