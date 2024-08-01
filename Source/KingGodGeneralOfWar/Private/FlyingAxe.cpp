// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingAxe.h"
#include "Components/ArrowComponent.h"
// Sets default values
AFlyingAxe::AFlyingAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	SubMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMeshComp"));
	SubMeshComp->SetupAttachment(MeshComp);

	DirectionArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrowComp"));
	DirectionArrowComp->SetupAttachment(MeshComp);

	RotationAxisComp = CreateDefaultSubobject<UArrowComponent>(TEXT("RotationAxisComp"));
	RotationAxisComp->SetupAttachment(MeshComp);
	RotationAxisComp->SetRelativeRotation(FRotator(0, 90, 0));
}

// Called when the game starts or when spawned
void AFlyingAxe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyingAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + DirectionArrowComp->GetForwardVector() * MoveSpeed * DeltaTime);

	FQuat quat = FRotator(-20, 0, 0).Quaternion();
	SubMeshComp->AddRelativeRotation(quat);
}

