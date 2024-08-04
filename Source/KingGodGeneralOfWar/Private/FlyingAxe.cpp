// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingAxe.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kratos.h"
#include "Axe.h"
#include "Kismet/KismetMathLibrary.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorFSM.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "Kismet/GameplayStatics.h"
const float LerpInit = 0.02;
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
	Player = Cast<AKratos>(GetWorld()->GetFirstPlayerController()->GetPawn());
	LerpAlpha = LerpInit;
}

// Called every frame
void AFlyingAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Player) return;

	// 날아가는 중
	if (!(isHit || isWithdraw))
	{
		SetActorLocation(GetActorLocation() + DirectionArrowComp->GetForwardVector() * MoveSpeed * DeltaTime);

		FQuat quat = FRotator(-45, 1, 1).Quaternion();
		SubMeshComp->AddRelativeRotation(quat);
	}
	// 회수 중
	else if (isWithdraw)
	{
		// 회수: 플레이어에게 돌아오는 모션
		if (!isRising)
		{
			TargetLocation = Player->GetMesh()->GetSocketLocation(TEXT("hand_rAxeSocket"));
		}

		if (FVector::Dist(GetActorLocation(), TargetLocation) < 30)
		{
			if (isRising)
			{
				isRising = false;
				LerpAlpha = LerpInit;
				CurLocation = GetActorLocation();
			}
			else
			{
				Player->CurrentWeapon->MeshComp->SetVisibility(true, true);
				Player->bAxeGone = false;
				Player->bIsAxeWithdrawing = false;
				this->Destroy();
			}
		}
		else
		{
			if (isRising)
			{
				FQuat quat = FRotator(-10, 0, 0).Quaternion();
				SubMeshComp->AddRelativeRotation(quat);
				SetActorLocation(FMath::Lerp(GetActorLocation(), TargetLocation, DeltaTime * 4));
			}
			else
			{
				FQuat quat = FRotator(WithdrawRotationScale, 0.1f, 0.1f).Quaternion();
				WithdrawRotationScale -= 0.33;
				SubMeshComp->AddRelativeRotation(quat);
				SetActorLocation(FMath::Lerp(CurLocation, TargetLocation, LerpAlpha));
			}
		}
		LerpAlpha += LerpInit;
		if (LerpAlpha >= 1)
		{
			CurLocation = GetActorLocation();
			LerpAlpha = 0.9;
		}
	}
}

void AFlyingAxe::FlyingAxeOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isHit = true;
	auto* Thor = Cast<ABDThor>(OtherActor);
	SetActorEnableCollision(false); // 충돌 비활성화

	if (Thor)
	{
		Thor->fsm->Damage(10);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
		AttachToComponent(Thor->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);

	}
	else
	{
		auto AwakenThor = Cast<AAwakenThor>(OtherActor);
		if (AwakenThor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, UEnum::GetValueAsString(AttackTypeDirectionArr[static_cast<int8>(EAttackType::AXE_THROW_ATTACK)][isWithdraw]));
			AwakenThor->getFSM()->SetDamage(5, AttackTypeDirectionArr[static_cast<int8>(EAttackType::AXE_THROW_ATTACK)][isWithdraw]);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
			AttachToComponent(AwakenThor->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	if (!isWithdraw)
	{
		SubMeshComp->SetRelativeRotation(HitArrowComp->GetRelativeRotation());
	}
}

void AFlyingAxe::WithdrawAxe()
{
	TargetLocation = WithdrawTargetPosition->GetComponentLocation();
	SubMeshComp->SetRelativeRotation(WithdrawRotation->GetRelativeRotation());
	isWithdraw = true;
	isRising = true;
	CurLocation = GetActorLocation();
	this->SetActorEnableCollision(true); // 충돌 비활성화
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

