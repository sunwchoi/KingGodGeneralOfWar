// Fill out your copyright notice in the Description page of Project Settings.


#include "RuneAttackField.h"
#include "Components/BoxComponent.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorFSM.h"
// Sets default values
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
		}, 2.5f, false);
}

// Called every frame
void ARuneAttackField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARuneAttackField::OnFieldOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* Thor = Cast<ABDThor>(OtherActor);
	const float fieldDamage = 8;
	const float fieldDalay = 0.3;
	if (Thor)
	{
		Thor->fsm->Damage(fieldDamage);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [Thor, fieldDamage, fieldDalay]()
			{
				Thor->fsm->Damage(fieldDamage);
				FTimerHandle handle1;
				Thor->GetWorld()->GetTimerManager().SetTimer(handle1, [Thor, fieldDamage, fieldDalay]()
					{
						Thor->fsm->Damage(fieldDamage);

					}, fieldDalay, false);
			}, fieldDalay, false);
	}
	else
	{
		auto AwakenThor= Cast<AAwakenThor>(OtherActor);
		AwakenThor->getFSM()->SetDamage(fieldDamage);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [AwakenThor, fieldDamage, fieldDalay]()
			{
				AwakenThor->getFSM()->SetDamage(fieldDamage);
				FTimerHandle handle1;
				AwakenThor->GetWorld()->GetTimerManager().SetTimer(handle1, [AwakenThor, fieldDamage, fieldDalay]()
					{
						AwakenThor->getFSM()->SetDamage(fieldDamage);

					}, fieldDalay, false);
			}, fieldDalay, false);
	}
}

