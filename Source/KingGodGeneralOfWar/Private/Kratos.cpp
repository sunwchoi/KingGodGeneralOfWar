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

	curHP = MaxHP;
	GetCharacterMovement()->MaxWalkSpeed = 1800;
	//Mesh->SetRelativeLocation(FVector(0, 0, -80));
	//Mesh->SetRelativeRotation(FRotator(0, 0, -90));
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

// Called every frame
void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();
	//GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Green, GetEnumValueAsString());
}
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
	case EPlayerState::Idle:
	case EPlayerState::Move:
		MoveScale = .33f;
		break;
	case EPlayerState::Run:
		MoveScale = 1.0f;
		break;
	case EPlayerState::Attack:
	case EPlayerState::Hit:
	case EPlayerState::Roll:
		break;
	case EPlayerState::Guard:
		MoveScale = .06f;
		break;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, ForwardDirection.ToString());
	AddMovementInput(ForwardDirection, MoveScale);
}

// Called to bind functionality to input
void AKratos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKratos::OnMyActionMove);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKratos::OnMyActionLook);
		input->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AKratos::OnMyActionRoll);
		input->BindAction(IA_Run, ETriggerEvent::Triggered, this, &AKratos::OnMyActionRunOn);
		input->BindAction(IA_Run, ETriggerEvent::Completed, this, &AKratos::OnMyActionRunOff);
		input->BindAction(IA_Guard, ETriggerEvent::Triggered, this, &AKratos::OnMyActionGuardOn);
		input->BindAction(IA_Guard, ETriggerEvent::Completed, this, &AKratos::OnMyActionGuardOff);
	}
}

void AKratos::OnMyActionMove(const FInputActionValue& Value)
{
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

void AKratos::OnMyActionLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);

}

void AKratos::OnMyActionRoll(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		State = EPlayerState::Roll;
		FTimerHandle RollTimerhandler;
		FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), GetControlRotation(), FVector(1, 1, 1));
		FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Direction);

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, ForwardDirection.ToString());
		ForwardDirection.Z = 0;
		LaunchCharacter(ForwardDirection * 3000.0f, true, false);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Roll!"));
		GetWorldTimerManager().SetTimer(RollTimerhandler, this, &AKratos::ExitRolling, 1.0f);
	}
}

void AKratos::OnMyActionRunOn(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Guard)
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
		State = EPlayerState::Guard;
	}
}

void AKratos::OnMyActionGuardOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Guard)
	{
		State = EPlayerState::Idle;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Run -> Move"));
	}
}

void AKratos::ExitRolling()
{
	if (State == EPlayerState::Roll)
	{
		State = EPlayerState::Idle;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("ExitRoll!"));

	}
}

void AKratos::Damage(int DamageValue, EAttackType AttackType)
{
	State = EPlayerState::Hit;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Hit!"));
}


