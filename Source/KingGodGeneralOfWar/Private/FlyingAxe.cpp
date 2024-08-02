// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingAxe.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kratos.h"
#include "Axe.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AFlyingAxe::AFlyingAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	SubMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMeshComp"));
	SubMeshComp->SetupAttachment(CapsuleComp);

	DirectionArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrowComp"));
	DirectionArrowComp->SetupAttachment(CapsuleComp);

	HitArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("HitArrowComp"));
	HitArrowComp->SetupAttachment(CapsuleComp);
	HitArrowComp->SetRelativeRotation(FRotator(40, 180, 0));

	WithdrawTargetPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPosition"));
	WithdrawTargetPosition->SetupAttachment(CapsuleComp);
	//WithdrawPosition->SetRelativeLocation(FVector());

	WithdrawRotation = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawRotation"));
	WithdrawRotation->SetupAttachment(CapsuleComp);
	WithdrawRotation->SetRelativeRotation(FRotator(-30, 180, 0));
}

// Called when the game starts or when spawned
void AFlyingAxe::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFlyingAxe::FlyingAxeOnComponentBeginOverlap);

}

// Called every frame
void AFlyingAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isHit && !isWithdraw)
	{
		SetActorLocation(GetActorLocation() + DirectionArrowComp->GetForwardVector() * MoveSpeed * DeltaTime);

		FQuat quat = FRotator(-30, 1, 1).Quaternion();
		SubMeshComp->AddRelativeRotation(quat);
	}
	else if (isWithdraw)
	{
		FTransform CurTransform = GetActorTransform();
		if (FVector::Dist(CurTransform.GetLocation(), TargetTransform.GetLocation()) < 10)
		{
			auto* player = Cast<AKratos>(GetWorld()->GetFirstPlayerController()->GetPawn());
			if (isRising)
			{
				TargetTransform = player->GetMesh()->GetSocketTransform(TEXT("hand_rAxeSocket"));
				isRising = false;
				mult = 12;
			}
			else
			{
				player->CurrentWeapon->MeshComp->SetVisibility(true, true);
				this->Destroy();
			}
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("플레이어로 오도록 전환"));
		}
		SetActorTransform(UKismetMathLibrary::TLerp(CurTransform, TargetTransform, DeltaTime * mult));
	}
}

void AFlyingAxe::FlyingAxeOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, OtherActor->GetName());
	isHit = true;
	SubMeshComp->SetRelativeRotation(HitArrowComp->GetRelativeRotation());
}

void AFlyingAxe::WithdrawAxe()
{
	TargetTransform = WithdrawTargetPosition->GetComponentTransform();
	SubMeshComp->SetRelativeRotation(WithdrawRotation->GetRelativeRotation());
	isWithdraw = true;
	isRising = true;
}

