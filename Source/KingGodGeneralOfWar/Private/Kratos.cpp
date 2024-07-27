// Fill out your copyright notice in the Description page of Project Settings.


#include "Kratos.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "TimerManager.h"
#include <GameFramework/CharacterMovementComponent.h>
// Sets default values

const float ATTACK1_DELAY = .9f;
const float ATTACK2_DELAY = 1.5f;
const float ATTACK3_DELAY = 1.5f;
const float ATTACK4_DELAY = 1.5f;
const float GUARD_START_DELAY = .3f;
const float GUARD_END_DELAY = .3f;
const float DODGE_DELAY = .5f;
AKratos::AKratos()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SocketOffset = FVector(0, 45, 50);
	SpringArmComp->TargetArmLength = 150;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	CurHP = MaxHP;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMaxSpeed;
}
// Called to bind functionality to input
void AKratos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKratos::OnMyActionMove);
		input->BindAction(IA_Move, ETriggerEvent::Completed, this, &AKratos::OnMyActionIdle);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKratos::OnMyActionLook);
		input->BindAction(IA_Dodge, ETriggerEvent::Started, this, &AKratos::OnMyActionDodge);
		input->BindAction(IA_Run, ETriggerEvent::Triggered, this, &AKratos::OnMyActionRunOn);
		input->BindAction(IA_Run, ETriggerEvent::Completed, this, &AKratos::OnMyActionRunOff);
		input->BindAction(IA_Guard, ETriggerEvent::Triggered, this, &AKratos::OnMyActionGuardOn);
		input->BindAction(IA_Guard, ETriggerEvent::Completed, this, &AKratos::OnMyActionGuardOff);
		input->BindAction(IA_LockOn, ETriggerEvent::Started, this, &AKratos::OnMyActionLockOn);
		input->BindAction(IA_Attack, ETriggerEvent::Started, this, &AKratos::OnMyActionAttack);
	}
}
// Called when the game starts or when spawned
void AKratos::BeginPlay()
{
	Super::BeginPlay();

	// 1. 컨트롤러를 가져와서 PlayerController인지 캐스팅해본다.
	auto* pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		// 2. 그 객체를 이용해서 EnhancedInputLocalPlayerSubsystem을 가져온다,
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}
}
// -------------------------------------------------- TICK -------------------------------------------------------------
// Called every frame
void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();
	if (bLockOn)
	{
		LockTargetFunc();
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, GetEnumValueAsString());
}
// -------------------------------------------------- TICK -------------------------------------------------------------

FString AKratos::GetEnumValueAsString()
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerState"), true);
	if (!EnumPtr) return TEXT("Invalid Enum");

	return EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(State)).ToString();
}
void AKratos::PlayerMove()
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0;
	FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), ControlRotation, FVector(1, 1, 1));
	FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Direction);

	Direction = FVector(0, 0, 0);

	float MoveScale = 0;
	switch (State)
	{
	case EPlayerState::Guard:
		MoveScale = .1f;
		break;
	case EPlayerState::Move:
		MoveScale = .5f;
		break;
	case EPlayerState::Run:
		MoveScale = 1.0f;
		break;
	}
	AddMovementInput(ForwardDirection, MoveScale);
}

FORCEINLINE void AKratos::LockTargetFunc()
{
	GetController()->AController::SetControlRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockTarget->GetActorLocation()));
}

void AKratos::OnMyActionMove(const FInputActionValue& Value)
{
	// 움직임은 유휴, 이동, 달리기, 가드시에만 가능
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run
		)
	{
		State = EPlayerState::Move;
		FVector2D v = Value.Get<FVector2D>();
		Direction.X = v.X;
		Direction.Y = v.Y;

		//Direction.Normalize();
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Direction.ToString());

		// 캐릭터 현재 회전 가져오기
		FRotator Rotation = GetControlRotation();
		FRotator YawRotation(0, Rotation.Yaw, 0);
		SetActorRotation(YawRotation);
	}
}

void AKratos::OnMyActionLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);

}

void AKratos::OnMyActionDodge(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		State = EPlayerState::Dodge;
		FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), GetControlRotation(), FVector(1, 1, 1));
		FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Direction);

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, ForwardDirection.ToString());
		ForwardDirection.Z = 0;
		LaunchCharacter(ForwardDirection * 3000.0f, true, false);

		FTimerHandle DodgeHandle;
		GetWorldTimerManager().SetTimer(DodgeHandle,
			// Roll -> Idle 전환
			[&]() {
			if (State == EPlayerState::Dodge)
			{
				State = EPlayerState::Idle;
				// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("ExitRoll!"));

			}
		},
			DODGE_DELAY, false);
	}
}
void AKratos::OnMyActionRunOn(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move)
	{
		State = EPlayerState::Run;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Move -> Run"));
	}
}

void AKratos::OnMyActionRunOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Run)
	{
		State = EPlayerState::Move;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Run -> Move"));
	}
}

void AKratos::OnMyActionGuardOn(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		State = EPlayerState::GuardStart;
		FTimerHandle guardHandle;

		GetWorld()->GetTimerManager().SetTimer(guardHandle,
			[&]()
		{
			if (State == EPlayerState::GuardStart)
			{
				State = EPlayerState::Guard;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("GuardCall Success"));

		}, GUARD_START_DELAY, false);
	}
}

void AKratos::OnMyActionGuardOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Guard || State == EPlayerState::GuardStart)
	{
		State = EPlayerState::GuardEnd;
		FTimerHandle guardHandle;

		GetWorld()->GetTimerManager().SetTimer(guardHandle,
			[&]()
		{
			State = EPlayerState::Idle;
		}, GUARD_END_DELAY / 2, false);
	}
}

void AKratos::OnMyActionLockOn(const FInputActionValue& value)
{
	if (bLockOn)
	{
		bLockOn = false;
		return;
	}

	float lockOnRadius = 500.0f;
	FVector cameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraComp->USceneComponent::K2_GetComponentRotation());
	FVector actorLocation = GetActorLocation() + cameraForwardVector * 500;
	FVector endLocation = GetActorLocation() + cameraForwardVector * 2000;
	float Radius = 500;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)));
	TArray<AActor*> ActorsToIgnore;
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	FHitResult OutHit;
	bool bIgnoreSelf = false;
	FLinearColor TraceColor = FLinearColor::White;
	FLinearColor TraceHitColor = FLinearColor::Red;
	float DrawTime = 10.0f;
	FCollisionObjectQueryParams ObjectQueryParams;

	bLockOn = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		actorLocation,
		endLocation,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHit,
		bIgnoreSelf,
		TraceColor,
		TraceHitColor,
		DrawTime
	);

	if (bLockOn)
	{
		LockTarget = OutHit.GetActor();
	}

}


void AKratos::OnMyActionIdle(const FInputActionValue& value)
{
	if (State == EPlayerState::Move || State == EPlayerState::Run)
		State = EPlayerState::Idle;
}

void AKratos::OnMyActionAttack(const FInputActionValue& value)
{
	if (bIsAttacking) return;

	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Guard
		|| State == EPlayerState::Run)
	{
		State = EPlayerState::MeleeAttack1;
		bIsAttacking = true;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Attack!"));

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			if (State == EPlayerState::MeleeAttack1)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Attack End"));
				this->State = EPlayerState::Idle;
				bIsAttacking = false;
			}
		},
			ATTACK1_DELAY, false);

	}
}



void AKratos::Damage(int DamageValue, EAttackType AttackType)
{
	State = EPlayerState::Hit;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Hit!"));
}


