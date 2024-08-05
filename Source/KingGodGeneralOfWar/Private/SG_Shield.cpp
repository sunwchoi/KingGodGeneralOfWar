// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_Shield.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "BDThor.h"
#include "CSW/AwakenThor.h"
#include "BDThorFSM.h"
#include "CSW/AwakenThorFSM.h"

const float SHIELD_DAMAGE = 2;

// Sets default values
ASG_Shield::ASG_Shield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	LightPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("LightPosition"));
	LightPosition->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ASG_Shield::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASG_Shield::OnShieldAttackOverlap);
}

// Called every frame
void ASG_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASG_Shield::OnShieldAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);

	auto* Thor = Cast<ABDThor>(OtherActor);

	if (Thor)
	{
		Thor->fsm->Damage(SHIELD_DAMAGE);
	}
	else
	{
		auto AwakenThor = Cast<AAwakenThor>(OtherActor);

		AwakenThor->getFSM()->SetDamage(SHIELD_DAMAGE, EAttackDirectionType::FORWARD);
	}

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 0.001f, false);
}

