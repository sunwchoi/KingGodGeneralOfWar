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
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
const float LerpInit = 0.022;
const float AXE_THROW_DAMAGE = 2;
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

	WithdrawTargetPosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPosition1"));
	WithdrawTargetPosition1->SetupAttachment(CapsuleComp);
	WithdrawTargetPosition1->SetRelativeLocation(FVector(-400, 0, 330));

	WithdrawTargetPosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPosition2"));
	WithdrawTargetPosition2->SetupAttachment(CapsuleComp);
	//WithdrawTargetPosition2->SetRelativeLocation(FVector());

	WithdrawTargetPosition3 = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPosition3"));
	WithdrawTargetPosition3->SetupAttachment(CapsuleComp);
	//WithdrawTargetPosition3->SetRelativeLocation(FVector());

	WithdrawRotation = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawRotation"));
	WithdrawRotation->SetupAttachment(CapsuleComp);
	WithdrawRotation->SetRelativeRotation(FRotator(-30, 180, 0));
}

// Called when the game starts or when spawned
void AFlyingAxe::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFlyingAxe::FlyingAxeOnComponentBeginOverlap);
	Me = Cast<AKratos>(GetWorld()->GetFirstPlayerController()->GetPawn());
	LerpAlpha = LerpInit;
	WithdrawTargetPositionArr.Add(WithdrawTargetPosition1);
	WithdrawTargetPositionArr.Add(WithdrawTargetPosition2);
	WithdrawTargetPositionArr.Add(WithdrawTargetPosition3);

}

// Called every frame
void AFlyingAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Me) return;

	// 날아가는 중
	if (!(bHit || bWithdrawing))
	{
		SetActorLocation(GetActorLocation() + DirectionArrowComp->GetForwardVector() * MoveSpeed * DeltaTime);
		MoveSpeed += 10;
		FQuat quat = FRotator(-45, 2, 2).Quaternion();
		SubMeshComp->AddRelativeRotation(quat);
	}
	// 회수 중
	else if (bWithdrawing)
	{
		// 회수: 플레이어에게 돌아오는 모션
		if (!bRising)
		{
			TargetLocation = Me->WithdrawPositionComp->GetComponentLocation();
		}
		const float MIN_DIST = 30;
		if (FVector::Dist(GetActorLocation(), TargetLocation) < MIN_DIST)
		{
			if (bRising)
			{
				bRising = false;
				LerpAlpha = LerpInit;
				CurLocation = GetActorLocation();
			}
			else
			{
				if (Me->State != EPlayerState::Hit)
				{
					Me->CatchFlyingAxe();
					this->Destroy();
				}
				else
				{
					FQuat quat = FRotator(WithdrawRotationScale, -1.0f, -2.0f).Quaternion();
					WithdrawRotationScale -= 0.33;
					SubMeshComp->AddRelativeRotation(quat);
				}
			}
		}
		else
		{
			if (bRising)
			{
				FQuat quat = FRotator(-15, 0, 1).Quaternion();
				SubMeshComp->AddRelativeRotation(quat);
				SetActorLocation(FMath::Lerp(GetActorLocation(), TargetLocation, DeltaTime * 6));
			}
			else
			{
				FQuat quat = FRotator(WithdrawRotationScale, -1.0f, -2.0f).Quaternion();
				//FQuat quat = FRotator(WithdrawRotationScale, -1.0f, -2.0f).Quaternion();
				WithdrawRotationScale -= 0.33;
				SubMeshComp->AddRelativeRotation(quat);
				FVector nextLocation = FMath::Lerp(CurLocation, TargetLocation, LerpAlpha);
				SetActorLocation(nextLocation);
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
	bHit = true;
	SetActorEnableCollision(false); // 충돌 비활성화
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("SetActorEnableCollision(false)")));
	auto* Thor = Cast<ABDThor>(OtherActor);
	if (Thor)
	{
		Thor->fsm->Damage(AXE_THROW_DAMAGE, AttackTypeDirectionArr[static_cast<int8>(EAttackType::AXE_THROW_ATTACK)][bWithdrawing]);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		auto AwakenThor = Cast<AAwakenThor>(OtherActor);
		if (AwakenThor)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, UEnum::GetValueAsString(AttackTypeDirectionArr[static_cast<int8>(EAttackType::AXE_THROW_ATTACK)][isWithdraw]));
			AwakenThor->getFSM()->SetDamage(AXE_THROW_DAMAGE, AttackTypeDirectionArr[static_cast<int8>(EAttackType::AXE_THROW_ATTACK)][bWithdrawing]);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
			AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	if (!bWithdrawing)
	{
		SubMeshComp->SetRelativeRotation(HitArrowComp->GetRelativeRotation());
	}
}

void AFlyingAxe::BackToPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("BackToPlayer")));
	int randIdx = FMath::RandRange(0, 2);
	TargetLocation = WithdrawTargetPositionArr[randIdx]->GetComponentLocation();
	SubMeshComp->SetRelativeRotation(WithdrawRotation->GetRelativeRotation());
	bWithdrawing = true;
	bRising = true;
	CurLocation = GetActorLocation();
	this->SetActorEnableCollision(true); // 충돌 비활성화
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

