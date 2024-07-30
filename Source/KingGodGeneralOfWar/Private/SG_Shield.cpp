// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_Shield.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ASG_Shield::ASG_Shield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

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
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Shield Hit!!"));
	MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 0.002f, false);
}

