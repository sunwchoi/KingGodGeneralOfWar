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
	if (Thor)
	{
		Thor->fsm->Damage(20);
	}
	else
	{
		auto AwakenThor= Cast<AAwakenThor>(OtherActor);
		AwakenThor->getFSM()->SetDamage(20);
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Thor get damage 20")));
}

