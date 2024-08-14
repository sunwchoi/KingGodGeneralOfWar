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
#include "Components/ArrowComponent.h"
#include "PlayerHPUI.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorFSM.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "FlyingAxe.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "CSW/CSWGameMode.h"
#include "Sound/SoundBase.h"

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
const float PARRY_FOV = 100;
const float ATTACK_FOV = 105;
const int GUARD_MAX_COUNT = 2;

const float PARRYING_DAMAGE = 3;

AKratos::AKratos()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SocketOffset = DefaultCameraOffset;
	SpringArmComp->TargetArmLength = DefaultTargetTargetArmLength;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	WithdrawPositionComp = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPositionComp"));
	WithdrawPositionComp->SetupAttachment(RootComponent);

	CurHP = MaxHP;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMaxSpeed;

	// Sound Referencing
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> avoid_sfx(TEXT("/ Script / Engine.SoundWave'/Game/JSG/SFX/RealSFX/Avoid.Avoid'"));
		if (avoid_sfx.Succeeded())	AvoidSound = avoid_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> axeThrow_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Axe2Throw.Axe2Throw'"));
		if (axeThrow_sfx.Succeeded())	AxeThrowSound = axeThrow_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> AxeWithdraw_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Axe3Get.Axe3Get'"));
		if (AxeWithdraw_sfx.Succeeded())	AxeWithdrawSound = AxeWithdraw_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> hit1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/PlayerDamaged3.PlayerDamaged3'"));
		if (hit1_sfx.Succeeded())	HitSound1 = hit1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> hit2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/PlayerDamaged.PlayerDamaged'"));
		if (hit2_sfx.Succeeded())	HitSound2 = hit2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> hit3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/PlayerDamaged2.PlayerDamaged2'"));
		if (hit3_sfx.Succeeded())	HitSound3 = hit3_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> roll_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Roll.Roll'"));
		if (roll_sfx.Succeeded())	RollSound = roll_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> runeBase_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/RuneReady.RuneReady'"));
		if (runeBase_sfx.Succeeded())	RuneBaseSound = runeBase_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune1.Rune1'"));
		if (rune1_sfx.Succeeded())	RuneAttack1Sound = rune1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune2.Rune2'"));
		if (rune2_sfx.Succeeded())	RuneAttack2Sound = rune2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune3.Rune3'"));
		if (rune3_sfx.Succeeded())	RuneAttack3Sound = rune3_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune4_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune4.Rune4'"));
		if (rune4_sfx.Succeeded())	RuneAttack4Sound = rune4_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack1.StrongAttack1'"));
		if (strong1_sfx.Succeeded())	StrongAttack1Sound = strong1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack2.StrongAttack2'"));
		if (strong2_sfx.Succeeded())	StrongAttack2Sound = strong2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack3.StrongAttack3'"));
		if (strong3_sfx.Succeeded())	StrongAttack3Sound = strong3_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong4_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack4.StrongAttack4'"));
		if (strong4_sfx.Succeeded())	StrongAttack4Sound = strong4_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> weak1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/WeakAttack1.WeakAttack1'"));
		if (weak1_sfx.Succeeded())	WeakAttack1Sound = weak1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> weak2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/WeakAttack2.WeakAttack2'"));
		if (weak2_sfx.Succeeded())	WeakAttack2Sound = weak2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> weak3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/WeakAttack3.WeakAttack3'"));
		if (weak3_sfx.Succeeded())	WeakAttack3Sound = weak3_sfx.Object;

		WeakAttackSoundArr.Add(WeakAttack1Sound);
		WeakAttackSoundArr.Add(WeakAttack1Sound);
		WeakAttackSoundArr.Add(WeakAttack2Sound);
		WeakAttackSoundArr.Add(WeakAttack3Sound);

		StrongAttackSoundArr.Add(StrongAttack1Sound);
		StrongAttackSoundArr.Add(StrongAttack1Sound);
		StrongAttackSoundArr.Add(StrongAttack2Sound);
		StrongAttackSoundArr.Add(StrongAttack3Sound);
		StrongAttackSoundArr.Add(StrongAttack4Sound);

		RuneAttackSoundArr.Add(RuneAttack1Sound);
		RuneAttackSoundArr.Add(RuneAttack1Sound);
		RuneAttackSoundArr.Add(RuneAttack2Sound);
		RuneAttackSoundArr.Add(RuneAttack3Sound);
		RuneAttackSoundArr.Add(RuneAttack4Sound);

	}
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
		Anim->Me = this;
		// 공격이 유효할 시점을 체크하는 노티파이 AttackHitCheck
		// 콜리전 설정을 On -> Off로 전환
		Anim->OnAttackHitCheck.AddLambda([this]() -> void
			{
				if (CurrentAttackType == EAttackType::STRONG_ATTACK)
				{
					// 방패 공격 : 강공격 콤보 3번쨰
					if (CurrentStrongCombo == 3)
						Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("HitableWeapon"), true);
					// 도끼 공격
					else
						Axe->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("HitableWeapon"), true);
				}
				else if (CurrentAttackType == EAttackType::DASH_ATTACK)
				{
					Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("HitableWeapon"), true);
				}
				else if (CurrentAttackType == EAttackType::RUNE_ATTACK || CurrentAttackType == EAttackType::WEAK_ATTACK)
				{
					Axe->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("HitableWeapon"), true);
				}
			});
		// 공격이 끝난 시점을 체크하는 노티파이 AttackEndCheck
		// 콜리전 설정을 Off -> On로 전환
		Anim->OnAttackEndCheck.AddLambda([this]() -> void
			{
				if (CurrentAttackType == EAttackType::STRONG_ATTACK)
				{
					if (CurrentStrongCombo == 3)
						Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
					else
						Axe->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
				}
				else if (CurrentAttackType == EAttackType::DASH_ATTACK)
				{
					Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
				}
				else if (CurrentAttackType == EAttackType::RUNE_ATTACK || CurrentAttackType == EAttackType::WEAK_ATTACK)
				{
					Axe->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
				}

			});
		Anim->OnNextAttackCheck.AddLambda([this]() -> void
			{
				CanNextStrongCombo = false;

				if (bIsStrongComboInputOn)
				{
					StrongAttackStartComboState();
					Anim->JumpToAttackMontageSection(CurrentStrongCombo);
					UGameplayStatics::PlaySound2D(GetWorld(), StrongAttackSoundArr[CurrentStrongCombo], 1, 1, 0.55);
				}
			});
		Anim->OnNextWeakAttackCheck.AddLambda([this]() -> void
			{
				CanNextWeakCombo = false;

				if (bIsWeakComboInputOn)
				{
					WeakAttackStartComboState();
					Anim->JumpToAttackMontageSection(CurrentWeakCombo);
					if (CurrentAttackType == EAttackType::WEAK_ATTACK)
					{
						if (CurrentWeakCombo <= 2)
							UGameplayStatics::PlaySound2D(GetWorld(), WeakAttackSoundArr[CurrentWeakCombo], 1, 1, 1.55f);
						//else
							//UGameplayStatics::PlaySound2D(GetWorld(), WeakAttackSoundArr[CurrentWeakCombo], 1, 1, .3f);

					}
					else
					{
						if (CurrentWeakCombo <= 3)
							UGameplayStatics::PlaySound2D(GetWorld(), RuneAttackSoundArr[CurrentWeakCombo], 1, 1, 0.34);
						else
							UGameplayStatics::PlaySound2D(GetWorld(), RuneAttackSoundArr[CurrentWeakCombo], 1, 1, 0.65f);

					}

				}
			});
		Anim->OnMovableCheck.AddLambda([this]() -> void
			{
				//SetState(EPlayerState::Idle);
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
	if (AimWidgetClass)
	{
		AimWidget = CreateWidget<UUserWidget>(GetWorld(), AimWidgetClass);
		if (AimWidget)
		{
			AimWidget->AddToViewport();
			AimWidget->SetVisibility(ESlateVisibility::Hidden);
		}

	}

	if (pc)
	{
		// 2. 그 객체를 이용해서 EnhancedInputLocalPlayerSubsystem을 가져온다,
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}
	GameMode = Cast<ACSWGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr == Axe)
	{
		SetWeapon();
	}

	if (nullptr == Shield)
	{
		SetShield();
	}
	GuardHitCnt = GUARD_MAX_COUNT;

	/*HpBarUI = CreateWidget<UPlayerHPUI>(GetWorld(), HpBarUIFactory);
	HpBarUI->AddToViewport();*/

	AttackShakeFactoryArr.Add(DownAttackShakeFactory);
	AttackShakeFactoryArr.Add(UpAttackShakeFactory);
	AttackShakeFactoryArr.Add(LeftAttackShakeFactory);
	AttackShakeFactoryArr.Add(RightAttackShakeFactory);

}
// -------------------------------------------------- TICK -------------------------------------------------------------
// Called every frame
void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();
	LockTargetFunc(DeltaTime);
	FRotator playerRotation = GetActorRotation();

	// 플레이어 로테이션 선형 보간
	SetActorRotation(UKismetMathLibrary::RLerp(playerRotation, TargetActorRotation, DeltaTime * 4, true));

	// 카메라 시야각 선형 보간
	CameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, TargetFOV, DeltaTime * 3);

	//카메라 오육칠팔프셋 선동생 간보 
	// 카메라 오프셋 선형 보간
	SpringArmComp->SocketOffset = FMath::Lerp(SpringArmComp->SocketOffset, TargetCameraOffset, DeltaTime * 2);

	// 쉴드 스케일 선형 보간. (가드시 커짐, 평소 스케일 0) 
	Shield->MeshComp->SetWorldScale3D(FVector(FMath::Lerp(Shield->MeshComp->GetComponentScale().X, TargetGuardScale, DeltaTime * 8)));

	// TargetArmLength 선형 보간
	SpringArmComp->TargetArmLength = FMath::Lerp(SpringArmComp->TargetArmLength, TargetTargetArmLength, DeltaTime * 2);

	// 카메라 앵글 선형 보간
	CameraComp->SetRelativeRotation(UKismetMathLibrary::RLerp(CameraComp->GetRelativeRotation(), TargetCameraAngle, DeltaTime * 2, true));

	if (bRuneReady)
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("RuneReady")));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("CurrentAttackType: %s"), *UEnum::GetValueAsString(CurrentAttackType)));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, GetPlayerStateString());
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("TargetTargetArmLength: %f"), TargetTargetArmLength));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("TargetCameraOffset: %s"), *TargetCameraOffset.ToString()));

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
	if (!LockTarget)
	{
		bLockOn = false;
		return;
	}
	if (bLockOn)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, TEXT("LockSystem On"));

		FRotator playerCameraRotation = GetController()->AController::GetControlRotation();
		TargetCameraRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockTarget->GetActorLocation());
		FRotator ToCameraRotation = UKismetMathLibrary::RLerp(playerCameraRotation, TargetCameraRotation, DeltaTime * 3, true);
		GetController()->AController::SetControlRotation(FRotator(playerCameraRotation.Pitch, ToCameraRotation.Yaw, playerCameraRotation.Roll));
	}
}

void AKratos::OnMontageEndedDelegated(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		if (State == EPlayerState::Hit)
		{

		}
		else if (Montage == Anim->DodgeMontage && State == EPlayerState::Roll)
		{
			return;
		}
		else if (Montage == Anim->DodgeMontage && CurrentAttackType == EAttackType::DASH_ATTACK)
		{
			return;
		}
		else if (Montage == Anim->GuardMontage && bGuardStagger)
		{
			return;
		}
		else if (Montage == Anim->GuardMontage && State == EPlayerState::Attack)
		{
			return;
		}
		else
		{
			WeakAttackEndComboState();
			StrongAttackEndComboState();
			bIsAttacking = false;
			bParrying = false;
			bIsDodging = false;
			bGuardStagger = false;
			bSuperArmor = false;
		}
	}
	else
	{
		SetState(EPlayerState::Idle);
	}
}

void AKratos::OnMyGuardDisappear()
{
	TargetGuardScale = 0;
}

void AKratos::OnMyLaunchCharacterInStrongAttack()
{
	const float LaunchScale = 500;
	LaunchCharacter(GetActorForwardVector() * LaunchScale, true, false);
}

void AKratos::OnMyJumpCharacterInStrongAttack()
{
	const float LaunchScale = 250;
	FVector dir = GetActorForwardVector() + FVector(0, 0, 1);
	dir.Normalize();
	LaunchCharacter(dir * LaunchScale, true, true);
}

void AKratos::IncreaseTargetTargetArmLength(float value)
{
	TargetTargetArmLength += value;
}

void AKratos::IncreaseTargetCameraOffset(FVector value)
{
	TargetCameraOffset += value;
}

void AKratos::SetWeapon()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Axe = GetWorld()->SpawnActor<AAxe>(AxeFactory, GetMesh()->GetSocketTransform(TEXT("hand_rAxeSocket")), param);
	if (Axe)
	{
		Axe->K2_AttachToComponent(GetMesh(), TEXT("hand_rAxeSocket"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		Axe->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
		Axe->Me = this;
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
		Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
	}
	Shield->MeshComp->SetWorldScale3D(FVector(0));
}

void AKratos::OnMyRuneReady()
{
	bRuneReady = true;

}

void AKratos::OnMyRuneAttackEnd()
{
	WeakAttackEndComboState();
	bRuneReady = false;
	bSuperArmor = false;
	bZoomOut = false;
	CurrentAttackType = EAttackType::NONE;
}

void AKratos::OnMyRuneAttackCameraSet()
{
	TargetFOV = 100;
	TargetCameraOffset = DefaultCameraOffset + FVector(0, 20, 55);
	TargetCameraAngle = DefaultCameraAngle + FRotator(-10, 0, 0);
	TargetTargetArmLength = TargetTargetArmLength + 30;
}

void AKratos::OnMySpawnEarthCrack()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EarthCrackVFX, Axe->MeshComp->GetComponentLocation() - FVector(0, 0, 22));
}

void AKratos::OnMyAttackComboEnd()
{
	WeakAttackEndComboState();
	StrongAttackEndComboState();
	CurrentAttackType = EAttackType::NONE;
}

void AKratos::OnMyInitAttackType()
{
	CurrentAttackType = EAttackType::NONE;
}

void AKratos::OnMyAttackProgress()
{
	GetMovementComponent()->Velocity = GetActorForwardVector() * 2000;
}

void AKratos::OnMyEndWithFail()
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			GameMode->EndWithFail();
		}, 2.0f, false);
}

void AKratos::OnMyGetUPCameraSet()
{
	TargetCameraOffset = DefaultCameraOffset;
	TargetCameraAngle = DefaultCameraAngle;
	TargetTargetArmLength = DefaultTargetTargetArmLength;
}

void AKratos::CameraShakeOnAttack(EAttackDirectionType attackDir, float scale)
{

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(AttackShakeFactoryArr[static_cast<int8>(attackDir)], scale);
}

EAttackDirectionType AKratos::GetAttackDirection()
{
	int ComboCount = 0;
	if (CurrentAttackType == EAttackType::WEAK_ATTACK || CurrentAttackType == EAttackType::RUNE_ATTACK)
		ComboCount = CurrentWeakCombo;
	else if (CurrentAttackType == EAttackType::STRONG_ATTACK)
		ComboCount = CurrentStrongCombo;
	else
		ComboCount = 0;
	EAttackDirectionType attackDirection = AttackTypeDirectionArr[static_cast<int8>(CurrentAttackType)][ComboCount];

	return attackDirection;
}

int32 AKratos::GetCurrentWeakCombo()
{
	return CurrentWeakCombo;
}

int32 AKratos::GetCurrentStrongCombo()
{
	return CurrentStrongCombo;
}

void AKratos::SetGlobalTimeDilation(float Duration, float SlowScale)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowScale);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, Duration, false);
}

FString AKratos::GetHitSectionName(EHitType hitType)
{
	FString HitTypeValueAsString = UEnum::GetValueAsString(hitType);
	return HitTypeValueAsString.Mid(10);
}

FString AKratos::GetDodgeDirection(int& DodgeScale)
{
	float absX = abs(PrevDirection.X), absY = abs(PrevDirection.Y);
	FString DodgeDirString;
	DodgeScale = 2000;
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
			DodgeScale = 1500;
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
		DodgeScale = 1500;
	}
	return DodgeDirString;
}

void AKratos::OnMyActionMove(const FInputActionValue& Value)
{
	// 움직임은 유휴, 이동, 달리기가능
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		SetState(EPlayerState::Move);
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
	else if (State == EPlayerState::Attack)
	{
		FVector2D v = Value.Get<FVector2D>();
		Direction.X = v.X;
		Direction.Y = v.Y;
		Direction.Normalize();
		TargetActorRotation = FRotator(0, GetControlRotation().Yaw, 0);
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
	if (GetVelocity().Size() < 1 || State == EPlayerState::Hit || State == EPlayerState::NoneMovable)	return;

	if (!bIsDodging && State != EPlayerState::Roll)
	{
		SetState(EPlayerState::Dodge);
		bIsDodging = true;
		UGameplayStatics::PlaySound2D(GetWorld(), AvoidSound, 1, 1, 0.2f);

		int DodgeScale;
		FString DodgeDirString = GetDodgeDirection(DodgeScale);

		FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), GetControlRotation(), FVector(1, 1, 1));
		FVector DodgeDirection = UKismetMathLibrary::TransformDirection(T, PrevDirection);
		DodgeDirection.Z = 0;
		LaunchCharacter(DodgeDirection * DodgeScale, true, false);
		Anim->PlayDodgeMontage();
		Anim->JumpToDodgeMontageSection(DodgeDirString);
	}
	else if (State == EPlayerState::Dodge)
	{
		SetState(EPlayerState::Roll);
		UGameplayStatics::PlaySound2D(GetWorld(), RollSound);
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
		float RollScale = 1000;
		Anim->PlayRollMontage();
		Anim->JumpToRollMontageSection(sectionIdx);
	}
}

void AKratos::OnMyActionRunOn(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move)
	{
		SetState(EPlayerState::Run);

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Move -> Run"));
	}
}

void AKratos::OnMyActionRunOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Run)
	{
		SetState(EPlayerState::Move);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Run -> Move"));
	}
}

void AKratos::OnMyActionGuardOn(const FInputActionValue& value)
{
	if (bParrying || bGuardStagger)
	{
		return;
	}
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		SetState(EPlayerState::GuardStart);
		GuardHitCnt = GUARD_MAX_COUNT;
		Anim->PlayGuardMontage();
		TargetGuardScale = .1f;
	}
}

void AKratos::OnMyActionGuardOff(const FInputActionValue& value)
{
	bParrying = false;
	bGuardStagger = false;
	if (State == EPlayerState::Guard || State == EPlayerState::GuardStart)
	{
		Anim->Montage_Stop(0, Anim->GuardMontage);
		SetState(EPlayerState::Idle);
		TargetGuardScale = 0.0f;
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
		SetState(EPlayerState::Idle);
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
			SetState(EPlayerState::Attack);

			WeakAttackStartComboState();
			Anim->PlayWeakAttackMontage();
			Anim->JumpToAttackMontageSection(1);
			UGameplayStatics::PlaySound2D(GetWorld(), WeakAttackSoundArr[CurrentWeakCombo], 1, 1, 0.8f);

			bIsAttacking = true;
			CurrentAttackType = EAttackType::WEAK_ATTACK;
		}
		// 룬 공격
		else
		{
			bSuperArmor = true;
			SetState(EPlayerState::Attack);

			WeakAttackStartComboState();
			Anim->PlayRuneAttackMontage();
			Anim->JumpToAttackMontageSection(1);
			UGameplayStatics::PlaySound2D(GetWorld(), RuneAttackSoundArr[CurrentWeakCombo], 1, 1, 0.4f);
			bIsAttacking = true;
			CurrentAttackType = EAttackType::RUNE_ATTACK;
		}
	}
	// 도끼 던지기
	else if (State == EPlayerState::Aim)
	{
		if (Anim->PlayAxeThrowMontage())
		{
			CurrentAttackType = EAttackType::AXE_THROW_ATTACK;
			UGameplayStatics::PlaySound2D(GetWorld(), AxeThrowSound);

		}

	}
	// 대시 공격
	else if (State == EPlayerState::Dodge)
	{
		SetState(EPlayerState::Attack);
		LaunchCharacter(GetActorForwardVector() * 2000, false, false);
		Anim->PlayDashAttackMontage();
		CurrentAttackType = EAttackType::DASH_ATTACK;
	}
	// 패링 공격

	/*else if (State == EPlayerState::Parry)
	{
		SetState(EPlayerState::Attack);
		Anim->PlayParryAttackMontage();
	}*/
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
		SetState(EPlayerState::Attack);

		StrongAttackStartComboState();
		Anim->PlayStrongAttackMontage();
		Anim->JumpToAttackMontageSection(1);
		UGameplayStatics::PlaySound2D(GetWorld(), StrongAttackSoundArr[CurrentStrongCombo], 1, 1, 0.4f);
		bIsAttacking = true;
		CurrentAttackType = EAttackType::STRONG_ATTACK;
	}

}

void AKratos::OnMyActionAimOn(const FInputActionValue& value)
{
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		SetState(EPlayerState::Aim);
		AimWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AKratos::OnMyActionAimOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Aim)
	{
		SetState(EPlayerState::Idle);
		AimWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AKratos::OnMyActionWithdrawAxe(const FInputActionValue& value)
{
	if (bAxeGone && !bIsAxeWithdrawing)
	{
		WithdrawAxe();
		bIsAxeWithdrawing = true;
		UGameplayStatics::PlaySound2D(GetWorld(), AxeWithdrawSound, 1, 1, 0.07f);
	}
}

void AKratos::OnMyActionRuneBase(const FInputActionValue& value)
{
	if (bAxeGone) return;
	if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	{
		SetState(EPlayerState::NoneMovable);
		Anim->PlayRuneBaseMontage();
		UGameplayStatics::PlaySound2D(GetWorld(), RuneBaseSound, 1, 1, 0.2f);

	}
}

void AKratos::OnHideAxe()
{
	Axe->MeshComp->SetVisibility(false, true);
}

void AKratos::ThrowAxe(FRotator TargetRot)
{
	FlyingAxe = GetWorld()->SpawnActor<AFlyingAxe>(FlyingAxeFactory, Axe->GetActorLocation(), TargetRot);
	bAxeGone = true;
	CameraShakeOnAttack(EAttackDirectionType::UP, 1.0f);
}

void AKratos::WithdrawAxe()
{
	FlyingAxe->BackToPlayer();
}

void AKratos::CatchFlyingAxe()
{
	Anim->PlayAxeWithdrawMontage();
	Axe->MeshComp->SetVisibility(true, true);
	bAxeGone = false;
	bIsAxeWithdrawing = false;
	CurrentAttackType = EAttackType::NONE;

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CatchAxeShakeFactory, 0.3f);
}

void AKratos::SetState(EPlayerState NextState)
{
	//UE_LOG(LogTemp, Display, TEXT("%s -> %s"), *UEnum::GetValueAsString(State), *UEnum::GetValueAsString(NextState));
	switch (State)
	{
	case EPlayerState::Dodge:
		bIsDodging = false;
		break;

	case EPlayerState::Attack:
		bIsAttacking = false;

		break;
	case EPlayerState::Parry:
		bParrying = false;
		break;

	case EPlayerState::GuardStart:
	case EPlayerState::Guard:
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		break;
	}

	State = NextState;

	switch (NextState)
	{
	case EPlayerState::Idle:
		TargetFOV = DefaultTargetFOV;
		TargetCameraOffset = DefaultCameraOffset;
		TargetCameraAngle = FRotator(0);
		TargetTargetArmLength = 143;
		bIsDodging = false;
		bIsAttacking = false;
		break;
	case EPlayerState::Run:
		TargetFOV = RUN_FOV;
		break;
	case EPlayerState::Attack:
		TargetCameraOffset = FVector(0, 50, 77);
		//TargetFOV = ATTACK_FOV;
		break;
	case EPlayerState::Guard:
	case EPlayerState::GuardStart:
		TargetFOV = GUARD_FOV;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		break;
	case EPlayerState::Aim:
		TargetFOV = AIM_FOV;
		break;
	case EPlayerState::Parry:
		bParrying = true;
		TargetFOV = PARRY_FOV;
		break;
	case EPlayerState::Hit:
		WeakAttackEndComboState();
		StrongAttackEndComboState();
		bIsAttacking = false;
		bParrying = false;
		bIsDodging = false;
		bGuardStagger = false;
		bSuperArmor = false;
		break;
	default:
		TargetFOV = TargetFOV;
		break;
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

void AKratos::StrongAttackEndComboState()
{
	bIsStrongComboInputOn = false;
	CanNextStrongCombo = false;
	CurrentStrongCombo = 0;
}

bool AKratos::Damage(AActor* Attacker, int DamageValue, EHitType HitType, bool IsMelee)
{
	// 회피 상태
	if (State == EPlayerState::Dodge)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("회피 성공"));
	}
	// 가드 상태
	else if (State == EPlayerState::Guard)
	{
		Anim->JumpToGuardMontageSection(TEXT("Guard_Block"));
		// 가드 성공, 가드 카운트 --;
		if (GuardHitCnt >= 1)
		{
			LaunchCharacter(GetActorForwardVector() * -1 * 1500, true, false);
			GetWorld()->SpawnActor<AActor>(GuardBlockLightFactory, Shield->GetActorTransform())->AttachToActor(Shield, FAttachmentTransformRules::KeepWorldTransform);
			UNiagaraFunctionLibrary::SpawnSystemAttached(GuardBlockVFX, Shield->LightPosition, TEXT("GuardBlockVFX"), Shield->LightPosition->GetComponentLocation(), Shield->LightPosition->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
			GuardHitCnt -= 1;
			// 가드 성공 카메라 쉐이크
			
		}
		// 가드 크러쉬
		else
		{
			SetState(EPlayerState::NoneMovable);
			LaunchCharacter(GetActorForwardVector() * -1 * 3000, true, false);
			UNiagaraFunctionLibrary::SpawnSystemAttached(GuardCrashVFX, Shield->LightPosition, TEXT("GuardBlockVFX"), Shield->LightPosition->GetComponentLocation(), Shield->LightPosition->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
			Anim->JumpToGuardMontageSection(TEXT("Guard_Stagger"));
			GuardHitCnt = GUARD_MAX_COUNT;
			bGuardStagger = true;
			TargetGuardScale = 0.0f;
			// 가드 파괴 카메라 쉐이크
		}
	}
	// 패링 가능 상태
	else if (State == EPlayerState::GuardStart)
	{
		GetWorld()->SpawnActor<AActor>(ParryingLightFactory, Shield->LightPosition->GetComponentTransform())->AttachToActor(Shield, FAttachmentTransformRules::KeepWorldTransform);
		UNiagaraFunctionLibrary::SpawnSystemAttached(ParryVFX, Shield->LightPosition, TEXT("ParryVFX"), Shield->LightPosition->GetComponentLocation(), Shield->LightPosition->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryVFX, Shield->GetActorLocation(), FRotator(0), FVector(0.0001f));

		CameraShakeOnAttack(EAttackDirectionType::DOWN, 0.5f);
		Anim->JumpToGuardMontageSection(TEXT("Guard_Parrying"));
		SetState(EPlayerState::Parry);
		if (IsMelee)
		{
			auto* Thor = Cast<ABDThor>(Attacker);

			if (Thor)
			{
				Thor->fsm->Damage(PARRYING_DAMAGE, EAttackDirectionType::UP);
			}
			else
			{
				auto AwakenThor = Cast<AAwakenThor>(Attacker);

				AwakenThor->getFSM()->SetDamage(PARRYING_DAMAGE, EAttackDirectionType::UP, true);
			}
		}
	}
	else if (State == EPlayerState::Hit || State == EPlayerState::Parry || State == EPlayerState::Die)
	{

	}
	// 기본 피격
	else
	{
		//CurHP -= FMath::Max(DamageValue, 0);
		CurHP = FMath::Max(CurHP - DamageValue, 0);
		if (GameMode)
			GameMode->SetPlayerHpBar(CurHP / MaxHP);
		if (bSuperArmor) return false;
		if (CurHP == 0)
		{
			SetState(EPlayerState::Die);
			Anim->PlayHitMontage();
			Anim->JumpToHitMontageSection(TEXT("Death"));
			TargetCameraOffset = FVector(0, 50, -60);
			TargetCameraAngle = FRotator(10, 0, 0);
			TargetTargetArmLength = 180;
			CameraShakeOnAttack(EAttackDirectionType::DOWN, 1);

			return true;
		}

		Anim->PlayHitMontage();
		Anim->JumpToHitMontageSection(GetHitSectionName(HitType));
		UGameplayStatics::PlaySound2D(GetWorld(), HitSound2, 1, 1, 0.2f);
		if (HitType == EHitType::NB_HIGH)
		{
			TargetCameraOffset = FVector(0, 50, -60);
			TargetCameraAngle = FRotator(20, 0, 0);
			TargetTargetArmLength = 190;
			CameraShakeOnAttack(EAttackDirectionType::DOWN, 1);
		}
		else if (HitType == EHitType::STAGGER)
			CameraShakeOnAttack(EAttackDirectionType::DOWN, .1);
		else
			CameraShakeOnAttack(EAttackDirectionType::DOWN, .5);

		SetState(EPlayerState::Hit);
		return true;
	}
	return false;
}
