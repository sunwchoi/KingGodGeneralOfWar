// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/PoundThunderAttackZone.h"

#include "Components/CapsuleComponent.h"

APoundThunderAttackZone::APoundThunderAttackZone()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(RootComponent);
}

void APoundThunderAttackZone::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APoundThunderAttackZone::OnZoneBeginOverlap);

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() { this->Destroy(); }, 2.f, false);
}
