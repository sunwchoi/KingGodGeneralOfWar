// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Kratos.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AAxe::AAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	EdgeComp = CreateDefaultSubobject<UArrowComponent>(TEXT("EdgeComp"));
	EdgeComp->SetupAttachment(MeshComp);
	EdgeComp->SetRelativeLocation(FVector(-30, 0, 40));
	EdgeComp->SetRelativeScale3D(FVector(.4, .4, .4));

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
}

// Called when the game starts or when spawned
void AAxe::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnAxeBeginOverlap);
	MeshComp->OnComponentHit.AddDynamic(this, &AAxe::OnAxeHit);
}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAxe::OnAxeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
	TArray<AActor*> ActorsToIgnore;
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	FHitResult OutHit;
	bool bIgnoreSelf = false;
	FLinearColor TraceColor = FLinearColor::White;
	FLinearColor TraceHitColor = FLinearColor::Red;
	float DrawTime = 3.0f;
	FCollisionObjectQueryParams ObjectQueryParams;
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, SweepResult.ImpactPoint.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, SweepResult.BoneName.ToString());
	
	//UKismetSystemLibrary::LineTraceSingle(GetWorld(), SweepResult.ImpactPoint, OtherActor->GetActorLocation(),
	//	ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
	FTimerHandle handle;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, EdgeComp->GetComponentTransform());
	auto* player = Cast<AKratos>(GetOwner());
	if (player)
		player->CameraShakeOnAttack();
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 0.001f, false);
}

void AAxe::OnAxeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Hit.BoneName.ToString());
}

