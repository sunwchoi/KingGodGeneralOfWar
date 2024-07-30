// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AAxe::AAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	
}

// Called when the game starts or when spawned
void AAxe::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnAxeBeginOverlap);
}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAxe::OnAxeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
	FTimerHandle handle;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation(), FRotator(), FVector(.2f, .2f, .2f));
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 0.002f, false);
}

