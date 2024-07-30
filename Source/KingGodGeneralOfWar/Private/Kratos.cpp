// Fill out your copyright notice in the Description page of Project Settings.


#include "Kratos.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "TimerManager.h"
#include "SG_KratosAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Axe.h"
#include "SG_Shield.h"
// Sets default values

const float ATTACK1_DELAY = .7f;
const float ATTACK2_DELAY = 1.5f;
const float ATTACK3_DELAY = 1.5f;
const float ATTACK4_DELAY = 1.5f;

const float GUARD_START_DELAY = 0.3f;
const float GUARD_END_DELAY = 0.15f;
const float DODGE_DELAY = 0.5f;

const float WALK_FOV = 90;
const float RUN_FOV = 105;
const float GUARD_FOV = 70;
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
void AKratos::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Anim = Cast<USG_KratosAnim>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		// 공격 몽타주가 끝나면 실행할 함수: OnAttackMontageEnded
		Anim->OnMontageEnded.AddDynamic(this, &AKratos::OnAttackMontageEnded);

		// 닷지 몽타주가 끝나면 실행할 함수: OnDodgeMontageEnded
		Anim->OnMontageEnded.AddDynamic(this, &AKratos::OnDodgeMontageEnded);
		
		// 공격이 유효할 시점을 체크하는 노티파이 AttackHitCheck와 AttackEndCheck
		// 각각 콜리전 설정을 On -> Off로 전환
		
		Anim->OnAttackHitCheck.AddLambda([this]() -> void
			{
				if (CurrentCombo == 3)
					Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("Axe"), true);
				else
					CurrentWeapon->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("Axe"), true);
			});
		Anim->OnAttackEndCheck.AddLambda([this]() -> void
			{
				if (CurrentCombo == 3)
					Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
				else
					CurrentWeapon->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);

			});
		Anim->OnNextAttackCheck.AddLambda([this]() -> void
			{
				CanNextCombo = false;

				if (bIsComboInputOn)
				{
					AttackStartComboState();
					Anim->JumpToAttackMontageSection(CurrentCombo);
				}
			});
		
	}
}
// Called when the game starts or when spawned
void AKratos::BeginPlay()
{
	Super::BeginPlay();

	CurHP = MaxHP;
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

	if (nullptr == CurrentWeapon)
	{
		SetWeapon();
	}

	if (nullptr == Shield)
	{
		SetShield();
	}
}
// -------------------------------------------------- TICK -------------------------------------------------------------
// Called every frame
void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();
	LockTargetFunc(DeltaTime);
	//SetActorRotation(YawRotation);
	FRotator playerRotation = GetActorRotation();

	// 플레이어 로테이션 선형 보간
	SetActorRotation( UKismetMathLibrary::RLerp(playerRotation, TargetActorRotation, DeltaTime * 5, true));

	// 카메라 시야각 선형 보간
	CameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, TargetFOV, DeltaTime * 10);
	switch (State)
	{
	case EPlayerState::Run:
		TargetFOV = RUN_FOV;
		break;
	case EPlayerState::GuardStart:
	case EPlayerState::Guard:
		TargetFOV = GUARD_FOV;
		break;
	default:
		TargetFOV = WALK_FOV;
	}
	if (bLockOn)
	{
		TargetFOV -= 10;
	}

	/*GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(
		TEXT("TargetActorRotation: %s"), *TargetActorRotation.ToString()));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(
		TEXT("ActorRotation: %s"), *GetActorRotation().ToString()));*/

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, FString::Printf(
		TEXT("CurrentFOV: %f"), CameraComp->FieldOfView));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(
		TEXT("PLAYER_STATE: %s"), *GetPlayerStateString() ));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(
		TEXT("HP: %f"), CurHP));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(
		TEXT("CurrentSpeed: %lf"), GetVelocity().Size()));

}
// -------------------------------------------------- TICK -------------------------------------------------------------

FString AKratos::GetPlayerStateString()
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
	case EPlayerState::Move:
		MoveScale = .56f;
		break;
	case EPlayerState::Run:
		MoveScale = 1.0f;
		break;
	}
	AddMovementInput(ForwardDirection, MoveScale);
}

FORCEINLINE void AKratos::LockTargetFunc(float DeltaTime)
{
	if (bLockOn)
	{

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, TEXT("LockSystem On"));

		FRotator playerCameraRotation = GetController()->AController::GetControlRotation();
		//FRotator playerCameraYawRotation = FRotator()
		TargetCameraRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockTarget->GetActorLocation());
		FRotator ToCameraRotation = UKismetMathLibrary::RLerp(playerCameraRotation, TargetCameraRotation, DeltaTime * 15, true);
		GetController()->AController::SetControlRotation(FRotator(playerCameraRotation.Pitch, ToCameraRotation.Yaw, playerCameraRotation.Roll));
		/*GetController()->AController::SetControlRotation(
			UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockTarget->GetActorLocation()));*/
	}
}

void AKratos::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	State = EPlayerState::Idle;
	CanNextCombo = false;
	CurrentCombo = 0;
	bIsAttacking = false;
}

void AKratos::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	State = EPlayerState::Idle;
	bIsDodging = false;
}


void AKratos::SetWeapon()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AAxe>(AxeFactory, GetMesh()->GetSocketTransform(TEXT("hand_rAxeSocket")), param);
	if (CurrentWeapon)
	{
		CurrentWeapon->K2_AttachToComponent(GetMesh(), TEXT("hand_rAxeSocket"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
	}
}

void AKratos::SetShield()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Shield = GetWorld()->SpawnActor<ASG_Shield>(ShieldFactory, GetMesh()->GetSocketTransform(TEXT("hand_lShieldSocket")), param);
	if (Shield)
	{
		Shield->K2_AttachToComponent(GetMesh(), TEXT("hand_lShieldSocket"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);

	}
}

void AKratos::OnMyActionMove(const FInputActionValue& Value)
{
	// 움직임은 유휴, 이동, 달리기가능
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		if (State == EPlayerState::Guard)
			State = EPlayerState::Guard;
		else
			State = EPlayerState::Move;
		FVector2D v = Value.Get<FVector2D>();
		Direction.X = v.X;
		Direction.Y = v.Y;
		Direction.Normalize();
		//Direction.Normalize();
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Direction.ToString());

		// 캐릭터 현재 회전 가져오기
		//FRotator Rotation = GetControlRotation();
		TargetActorRotation = FRotator(0, GetControlRotation().Yaw, 0);
		//SetActorRotation(YawRotation);
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
	if (bIsDodging || GetVelocity().Size() < 1)	return;
	//if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run
	//	|| State == EPlayerState::Guard)
	{
		State = EPlayerState::Dodge;
		bIsDodging = true;
		
		FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), GetControlRotation(), FVector(1, 1, 1));
		//FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Direction);
		FVector ForwardDirection = GetVelocity();
		ForwardDirection.Z = 0;
		ForwardDirection.Normalize();
		float DodgeCoef = 1500;
		LaunchCharacter(ForwardDirection * DodgeCoef, true, true);
		Anim->PlayDodgeMontage();

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
		}, GUARD_END_DELAY, false);
	}
}

void AKratos::OnMyActionLockOn(const FInputActionValue& value)
{
	if (bLockOn)
	{
		bLockOn = false;
		return;
	}

	float lockOnRadius = 1000000.0f;
	FVector cameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraComp->USceneComponent::K2_GetComponentRotation());
	FVector actorLocation = GetActorLocation() + cameraForwardVector * 500;
	FVector endLocation = GetActorLocation() + cameraForwardVector * 5000;
	float Radius = 500;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)));
	TArray<AActor*> ActorsToIgnore;
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	FHitResult OutHit;
	bool bIgnoreSelf = false;
	FLinearColor TraceColor = FLinearColor::White;
	FLinearColor TraceHitColor = FLinearColor::Red;
	float DrawTime = 3.0f;
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
	if (bIsAttacking)
	{
		if (CanNextCombo)
		{
			bIsComboInputOn = true;
		}
		return;
	}

	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Guard
		|| State == EPlayerState::Run)
	{
		State = EPlayerState::MeleeAttack;

		AttackStartComboState();
		Anim->PlayAttackMontage();
		Anim->JumpToAttackMontageSection(CurrentCombo);
		bIsAttacking = true;
		
	}
}

void AKratos::AttackStartComboState()
{
	CanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int8>(CurrentCombo + 1, 1, MaxCombo);
}

void AKratos::AttackEndComboState()
{
	bIsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}


void AKratos::Damage(int DamageValue, EAttackType AttackType)
{
	//State = EPlayerState::Hit;
	CurHP -= DamageValue;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player Hit!"));
}


