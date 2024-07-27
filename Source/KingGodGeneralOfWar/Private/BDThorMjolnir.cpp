// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorMjolnir.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABDThorMjolnir::ABDThorMjolnir()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌 설정
	MjoCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MjoCol"));
	SetRootComponent(MjoCol);

	//메쉬설정
	MjolnirMesh = CreateDefaultSubobject<UStaticMeshComponent>("MjolnirMesh");
	MjolnirMesh->SetupAttachment(RootComponent);
	//메쉬 부착
	ConstructorHelpers::FObjectFinder <UStaticMesh> MeshComp(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));

	if (MeshComp.Succeeded()) {
		MjolnirMesh->SetStaticMesh(MeshComp.Object);
		MjolnirMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		MjolnirMesh->SetRelativeLocation(FVector(0, 0, -30.0f));

		//Location (X=20.000000,Y=-40.000000,Z=80.000000)
		// Rotation (Pitch=35.000000,Yaw=-115.000000,Roll=-170.000000)
		//scale(X=3.000000,Y=3.000000,Z=3.000000)
	}



}

// Called when the game starts or when spawned
void ABDThorMjolnir::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABDThorMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

