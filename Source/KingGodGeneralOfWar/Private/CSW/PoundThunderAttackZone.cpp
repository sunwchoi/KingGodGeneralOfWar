// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/PoundThunderAttackZone.h"

#include "Components/CapsuleComponent.h"

APoundThunderAttackZone::APoundThunderAttackZone()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp0 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp0"));
	CapsuleComp0->SetupAttachment(RootComponent);

	CapsuleComp1 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp1"));
	CapsuleComp1->SetupAttachment(RootComponent);


	CapsuleComp2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp2"));
	CapsuleComp2->SetupAttachment(RootComponent);

	
	CapsuleComp0->SetRelativeScale3D(FVector(5));
	CapsuleComp1->SetRelativeScale3D(FVector(5));
	CapsuleComp2->SetRelativeScale3D(FVector(5));
}

void APoundThunderAttackZone::Active()
{

	UE_LOG(LogTemp, Warning, TEXT("Active"));

	CapsuleComp0->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	CapsuleComp1->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	CapsuleComp2->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

}

void APoundThunderAttackZone::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComp0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CapsuleComp0->OnComponentBeginOverlap.AddDynamic(this, &APoundThunderAttackZone::OnZoneBeginOverlap);
	CapsuleComp1->OnComponentBeginOverlap.AddDynamic(this, &APoundThunderAttackZone::OnZoneBeginOverlap);
	CapsuleComp2->OnComponentBeginOverlap.AddDynamic(this, &APoundThunderAttackZone::OnZoneBeginOverlap);

	FTimerHandle activeHandle;
	FTimerHandle destroyHandle;
	GetWorld()->GetTimerManager().SetTimer(activeHandle, [this]() {this->Active();}, 1.f, false);
	GetWorld()->GetTimerManager().SetTimer(destroyHandle, [this]() {this->Destroy();}, 4.f, false);
}


