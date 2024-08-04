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
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
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
const float AIM_FOV = 60;

const int GUARD_MAX_COUNT = 3;
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

	if (AimWidgetClass)
		AimWidget = CreateWidget<UUserWidget>(GetWorld(), AimWidgetClass);

	if (AimWidget)
	{
		AimWidget->AddToViewport();
		AimWidget->SetVisibility(ESlateVisibility::Visible);
	}
	/*ConstructorHelpers::FObjectFinder<UWidgetComponent>TempAimWidgetComp(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/JSG/UI/WBP_Aim.WBP_Aim'"));
	if (TempAimWidgetComp.Succeeded())
	{
		AimWidgetComp = TempAimWidgetComp.Object;
	}*/
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

		input->BindAction(IA_WeakAttack, ETriggerEvent::Started, this, &AKratos::OnMyActionWeakAttack);
		input->BindAction(IA_StrongAttack, ETriggerEvent::Started, this, &AKratos::OnMyActionStrongAttack);
		input->BindAction(IA_Aim, ETriggerEvent::Triggered, this, &AKratos::OnMyActionAimOn);
		input->BindAction(IA_Aim, ETriggerEvent::Completed, this, &AKratos::OnMyActionAimOff);
		input->BindAction(IA_WithdrawAxe, ETriggerEvent::Started, this, &AKratos::OnMyActionWithdrawAxe);
		input->BindAction(IA_RuneBase, ETriggerEvent::Started, this, &AKratos::OnMyActionRuneBase);
		
	}
}
void AKratos::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Anim = Cast<USG_KratosAnim>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->OnMontageEnded.AddDynamic(this, &AKratos::OnMontageEndedDelegated);

		// 공격이 유효할 시점을 체크하는 노티파이 AttackHitCheck
		// 콜리전 설정을 On -> Off로 전환
		Anim->OnAttackHitCheck.AddLambda([this]() -> void
			{
				if (State == EPlayerState::StrongCombo)
				{
					// 방패 공격 : 강공격 콤보 3번쨰
					if (CurrentStrongCombo == 3)
						Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("Axe"), true);
					// 도끼 공격
					else 
						CurrentWeapon->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("Axe"), true);
				}
				else if (State == EPlayerState::DashAttack)
				{
					Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("Axe"), true);
				}
				else if (State == EPlayerState::RuneAttack || State == EPlayerState::WeakCombo)
				{
					CurrentWeapon->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("Axe"), true);
				}
			});
		// 공격이 끝난 시점을 체크하는 노티파이 AttackEndCheck
		// 콜리전 설정을 Off -> On로 전환
		Anim->OnAttackEndCheck.AddLambda([this]() -> void
			{
				if (State == EPlayerState::StrongCombo)
				{
					if (CurrentStrongCombo == 3)
						Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
					else
						CurrentWeapon->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
				}
				else if (State == EPlayerState::DashAttack)
				{
					Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
				}
				else if (State == EPlayerState::RuneAttack || State == EPlayerState::WeakCombo)
				{
					CurrentWeapon->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
				}

			});
		Anim->OnNextAttackCheck.AddLambda([this]() -> void
			{
				CanNextStrongCombo = false;

				if (bIsStrongComboInputOn)
				{
					StrongAttackStartComboState();
					Anim->JumpToAttackMontageSection(CurrentStrongCombo);
				}
			});
		Anim->OnNextWeakAttackCheck.AddLambda([this]() -> void
			{
				CanNextWeakCombo = false;
				
				if (bIsWeakComboInputOn)
				{
					WeakAttackStartComboState();
					Anim->JumpToAttackMontageSection(CurrentWeakCombo);
				}
			});
		Anim->OnMovableCheck.AddLambda([this]() -> void
			{
				State = EPlayerState::Idle;
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
	GuardHitCnt = GUARD_MAX_COUNT;
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
	case EPlayerState::Aim:
		TargetFOV = AIM_FOV;
		break;
	default:
		TargetFOV = WALK_FOV;
	}
	if (bLockOn)
	{
		TargetFOV -= 10;
	}
	if (bRuneReady)
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("RuneReady")));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, FString::Printf(TEXT("TargetFOV: %f"), TargetFOV));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, GetPlayerStateString() );

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(
		TEXT("HP: %f"), CurHP));
}
// -------------------------------------------------- TICK -------------------------------------------------------------

FString AKratos::GetPlayerStateString()
{
	return UEnum::GetValueAsString(State);
}
void AKratos::PlayerMove()
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0;
	FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), ControlRotation, FVector(1, 1, 1));
	FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Direction);

	PrevDirection = Direction;
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
		TargetCameraRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockTarget->GetActorLocation());
		FRotator ToCameraRotation = UKismetMathLibrary::RLerp(playerCameraRotation, TargetCameraRotation, DeltaTime * 15, true);
		GetController()->AController::SetControlRotation(FRotator(playerCameraRotation.Pitch, ToCameraRotation.Yaw, playerCameraRotation.Roll));
	}
}

void AKratos::OnMontageEndedDelegated(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == Anim->StrongAttackMontage)
	{
		State = EPlayerState::Idle;
		CanNextStrongCombo = false;
		CurrentStrongCombo = 0;
		bIsAttacking = false;
	}
	else if (Montage == Anim->WeakAttackMontage)
	{
		State = EPlayerState::Idle;
		WeakAttackEndComboState();
	}
	else if (Montage == Anim->DodgeMontage)
	{
		if (State == EPlayerState::Roll || State == EPlayerState::DashAttack) return;

		State = EPlayerState::Idle;
		bIsDodging = false;
	}
	else if (Montage == Anim->RollMontage)
	{
		State = EPlayerState::Idle;
		bIsDodging = false;
	}
	else if (Montage == Anim->GuardMontage)
	{
		if (bInterrupted)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("OnGuardMontageEnded On Interrupted"));
			if (Anim->Montage_IsPlaying(Anim->GuardMontage)) return;
			State = EPlayerState::Idle;
		}
		else
		{
			GuardHitCnt = GUARD_MAX_COUNT;
			State = EPlayerState::Idle;
		}
	}
	else if (Montage == Anim->DashAttackMontage)
	{
		State = EPlayerState::Idle;
		bIsDodging = false;

	}
	else if (Montage == Anim->RuneBaseMontage)
	{
		State = EPlayerState::Idle;
		bRuneReady = true;
	}
	else if (Montage == Anim->RuneAttackMontage)
	{
		State = EPlayerState::Idle;
		bRuneReady = false;
		WeakAttackEndComboState();
	}
	else
	{
	}
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

void AKratos::CameraShakeOnAttack()
{
	//FAddCameraShakeParams params();
	//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(AttackShake,);
}

void AKratos::OnMyActionMove(const FInputActionValue& Value)
{
	// 움직임은 유휴, 이동, 달리기가능
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		State = EPlayerState::Move;
		FVector2D v = Value.Get<FVector2D>();
		Direction.X = v.X;
		Direction.Y = v.Y;
		Direction.Normalize();
		TargetActorRotation = FRotator(0, GetControlRotation().Yaw, 0);
	}
	else if (State == EPlayerState::Dodge)
	{
		FVector2D v = Value.Get<FVector2D>();
		Direction.X = v.X;
		Direction.Y = v.Y;
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
	if (GetVelocity().Size() < 1)	return;

	if (!bIsDodging && State != EPlayerState::Roll)
	{
		State = EPlayerState::Dodge;
		bIsDodging = true;

		FString DodgeDirString;
		float absX = abs(PrevDirection.X), absY = abs(PrevDirection.Y);
		float DodgeCoef = 2000;

		if (absX <= 0.1)
		{
			if (PrevDirection.Y >= 0.9)
				DodgeDirString = TEXT("R");
			else
				DodgeDirString = TEXT("L");
		}
		else if (absY <= 0.1)
		{
			if (PrevDirection.X >= 0.9)
				DodgeDirString = TEXT("F");
			else
			{
				DodgeDirString = TEXT("B");
				DodgeCoef = 1500;
			}
		}
		else if (PrevDirection.X >= 0.5)
		{
			if (PrevDirection.Y >= 0.5)
				DodgeDirString = TEXT("RF");
			else
				DodgeDirString = TEXT("LF");
		}
		else
		{
			if (PrevDirection.Y >= 0.5)
				DodgeDirString = TEXT("RB");
			else
				DodgeDirString = TEXT("LB");
			DodgeCoef = 1500;
		}
		FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), GetControlRotation(), FVector(1, 1, 1));
		FVector DodgeDirection = UKismetMathLibrary::TransformDirection(T, PrevDirection);
		DodgeDirection.Z = 0;
		LaunchCharacter(DodgeDirection * DodgeCoef, true, false);
		Anim->PlayDodgeMontage();
		Anim->JumpToDodgeMontageSection(DodgeDirString);
	}
	else if (State == EPlayerState::Dodge)
	{
		State = EPlayerState::Roll;
		bIsDodging = true;
		Anim->Montage_Stop(0.1f, Anim->DodgeMontage);
		int8 sectionIdx = 3;
		if (PrevDirection.X == 0)
		{
			if (PrevDirection.Y == -1)
				sectionIdx = 0;
			else
				sectionIdx = 1;
		}
		else
		{
			if (PrevDirection.X == 1)
				sectionIdx = 2;
			else
				sectionIdx = 3;
		}
		Anim->PlayRollMontage();
		Anim->JumpToRollMontageSection(sectionIdx);
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
	if (bParrying) return;
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("OnMyActionGuardOn"));
		State = EPlayerState::GuardStart;
		Anim->PlayGuardMontage();
	}
}

void AKratos::OnMyActionGuardOff(const FInputActionValue& value)
{
	bParrying = false;
	if (State == EPlayerState::Guard || State == EPlayerState::GuardStart)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("OnMyActionGuardOff"));
		Anim->Montage_Stop(0, Anim->GuardMontage);
		//State = EPlayerState::GuardEnd;
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


void AKratos::OnMyActionWeakAttack(const FInputActionValue& value)
{
	if (bAxeGone) return;
	if (bIsAttacking)
	{
		if (CanNextWeakCombo)
		{
			bIsWeakComboInputOn = true;
		}
		return;
	}

	// 일반 약공격 콤보
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Guard
		|| State == EPlayerState::Run)
	{
		if (!bRuneReady)
		{
			State = EPlayerState::WeakCombo;

			WeakAttackStartComboState();
			Anim->PlayWeakAttackMontage();
			Anim->JumpToAttackMontageSection(1);
			bIsAttacking = true;
		}
		// 룬 공격
		else
		{
			State = EPlayerState::RuneAttack;

			WeakAttackStartComboState();
			Anim->PlayRuneAttackMontage();
			Anim->JumpToAttackMontageSection(1);
			bIsAttacking = true;
		}
	}
	// 도끼 던지기
	else if (State == EPlayerState::Aim)
	{
		bAxeGone = true;
		Anim->PlayAxeThrowMontage();
	}
	// 대시 공격
	else if (State == EPlayerState::Dodge)
	{
		State = EPlayerState::DashAttack;
		LaunchCharacter(GetActorForwardVector() * 1500, false, false);
		Anim->PlayDashAttackMontage();
	}
}

void AKratos::OnMyActionStrongAttack(const FInputActionValue& value)
{
	if (bAxeGone) return;
	if (bIsAttacking)
	{
		if (CanNextStrongCombo)
		{
			bIsStrongComboInputOn = true;
		}
		return;
	}

	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Guard
		|| State == EPlayerState::Run)
	{
		State = EPlayerState::StrongCombo;

		StrongAttackStartComboState();
		Anim->PlayStrongAttackMontage();
		Anim->JumpToAttackMontageSection(1);
		bIsAttacking = true;
	}
	
}

void AKratos::OnMyActionAimOn(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		State = EPlayerState::Aim;
	}
}

void AKratos::OnMyActionAimOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Aim)
	{
		State = EPlayerState::Idle;
	}
}

void AKratos::OnMyActionWithdrawAxe(const FInputActionValue& value)
{
	if (bAxeGone)
		Anim->PlayAxeWithdrawMontage();
}

void AKratos::OnMyActionRuneBase(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		State = EPlayerState::NoneMovable;
		Anim->PlayRuneBaseMontage();

	}
}

void AKratos::WeakAttackStartComboState()
{
	CanNextWeakCombo = true;
	bIsWeakComboInputOn = false;
	CurrentWeakCombo = FMath::Clamp<int8>(CurrentWeakCombo + 1, 1, 4);
}

void AKratos::WeakAttackEndComboState()
{
	bIsWeakComboInputOn = false;
	CanNextWeakCombo = false;
	CurrentWeakCombo = 0;
	bIsAttacking = false;
}

void AKratos::StrongAttackStartComboState()
{
	CanNextStrongCombo = true;
	bIsStrongComboInputOn = false;
	CurrentStrongCombo = FMath::Clamp<int8>(CurrentStrongCombo + 1, 1, 4);
}

void AKratos::StrongWeakAttackEndComboState()
{
	bIsStrongComboInputOn = false;
	CanNextStrongCombo = false;
	CurrentStrongCombo = 0;
}

void AKratos::Damage(int DamageValue, EHitType HitType, bool isMelee)
{
	if (State == EPlayerState::Dodge)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("회피 성공"));
	}
	else if (State == EPlayerState::Guard)
	{
		Anim->JumpToGuardMontageSection(TEXT("Guard_Block"));
		if (GuardHitCnt >= 1)
		{
			FVector PlayerToEnemyDirection = GetActorForwardVector();
			LaunchCharacter(PlayerToEnemyDirection * -1 * 1000, true, false);
			GuardHitCnt -= 1;
		}
		else
		{
			Anim->JumpToGuardMontageSection(TEXT("Guard_Stagger"));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("가드"));
	}
	else if (State == EPlayerState::GuardStart)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryVFX, Shield->GetActorLocation(), FRotator(0), FVector(1, 1, 1));
		Anim->JumpToGuardMontageSection(TEXT("Guard_Parrying"));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.06f);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [&]()
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			}, 0.02f, false);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("패링!!!!"));
		bParrying = true;
		State = EPlayerState::Parry;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("%d Damage!"), DamageValue));
		CurHP -= DamageValue;
	}
}