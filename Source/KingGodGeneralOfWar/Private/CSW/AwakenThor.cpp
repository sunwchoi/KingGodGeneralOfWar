// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThor.h"

#include "Kratos.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "CSW/AwakenThorFSM.h"
#include "CSW/HpBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AAwakenThor::AAwakenThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(25.f);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -110.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("/Script/Engine.AnimBlueprint'/Game/CSW/Blueprints/ABP_AwakenThor.ABP_AwakenThor_C'"));
	if (AnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletonMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/CSW/Asset/Model/AwakenThor.AwakenThor'"));
	if (skeletonMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skeletonMesh.Object);
	}
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	HeadCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCapsule"));
	HeadCapsule->SetupAttachment(GetMesh(), FName("Head"));
	HeadCapsule->SetCapsuleHalfHeight(90.f);
	HeadCapsule->SetCapsuleRadius(90.f);
	HeadCapsule->SetCollisionProfileName(TEXT("Enemy"));
	HeadCapsule->SetRelativeLocation(FVector(-0.000000f, -40.000002f, 0.000000f));
	
	LArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LArmCapsule"));
	LArmCapsule->SetupAttachment(GetMesh(), FName("LeftArm"));
	LArmCapsule->SetCapsuleHalfHeight(110.f);
	LArmCapsule->SetCapsuleRadius(70.f);
	LArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LArmCapsule->SetRelativeLocation(FVector(-0.000000f, -40.000002f, -0.000000f));
	LArmCapsule->SetRelativeRotation(FRotator(90.000000f, 26.565051f, -63.434949f));

	RArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RArmCapsule"));
	RArmCapsule->SetupAttachment(GetMesh(), FName("RightArm"));
	RArmCapsule->SetCapsuleHalfHeight(110.f);
	RArmCapsule->SetCapsuleRadius(70.f);
	RArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RArmCapsule->SetRelativeLocation(FVector(0.000000f, -40.000002f, 0.000000f));
	RArmCapsule->SetRelativeRotation(FRotator(90.000000f, 14.036243f, -75.963757f));
	
	LForeArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LForeArmCapsule"));
	LForeArmCapsule->SetupAttachment(GetMesh(), FName("LeftForeArm"));
	LForeArmCapsule->SetCapsuleHalfHeight(100.f);
	LForeArmCapsule->SetCapsuleRadius(60.f);
	LForeArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LForeArmCapsule->SetRelativeLocation(FVector(0.000000f, -120.000007f, -0.000000f));
	LForeArmCapsule->SetRelativeRotation(FRotator(90.000000f,-7.125016f,-97.125016f));

	RForeArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RForeArmCapsule"));
	RForeArmCapsule->SetupAttachment(GetMesh(), FName("RightForeArm"));
	RForeArmCapsule->SetCapsuleHalfHeight(100.f);
	RForeArmCapsule->SetCapsuleRadius(60.f);
	RForeArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RForeArmCapsule->SetRelativeLocation(FVector(-0.000000f,-120.000007f,-0.000000f));
	RForeArmCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f,0.000000f));

	RLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RLegCapsule"));
	RLegCapsule->SetupAttachment(GetMesh(), FName("RightLeg"));
	RLegCapsule->SetCapsuleHalfHeight(130.f);
	RLegCapsule->SetCapsuleRadius(70.f);
	RLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RLegCapsule->SetRelativeLocation(FVector(0.000000f, -80.000005f, 0.000000f));
	RLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f, 0.000000f));

	RUpLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RUpLegCapsule"));
	RUpLegCapsule->SetupAttachment(GetMesh(), FName("RightUpLeg"));
	RUpLegCapsule->SetCapsuleHalfHeight(160.f);
	RUpLegCapsule->SetCapsuleRadius(90.f);
	RUpLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RUpLegCapsule->SetRelativeLocation(FVector(-0.000000f, -80.000005f, 0.000000f));
	RUpLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f,0.000000f));

	LLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LLegCapsule"));
	LLegCapsule->SetupAttachment(GetMesh(), FName("LeftLeg"));
	LLegCapsule->SetCapsuleHalfHeight(130.f);
	LLegCapsule->SetCapsuleRadius(70.f);
	LLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LLegCapsule->SetRelativeLocation(FVector(0.000000f, -80.000005f, 0.000000f));
	LLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f, 0.000000f));

	LUpLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LUpLegCapsule"));
	LUpLegCapsule->SetupAttachment(GetMesh(), FName("LeftUpLeg"));
	LUpLegCapsule->SetCapsuleHalfHeight(160.f);
	LUpLegCapsule->SetCapsuleRadius(90.f);
	LUpLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LUpLegCapsule->SetRelativeLocation(FVector(-0.000000f, -80.000005f, 0.000000f));
	LUpLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f,0.000000f));

	GetCharacterMovement()->GravityScale = 15.f;
	GetCharacterMovement()->MaxAcceleration = 1000000.0f;
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	GetCharacterMovement()->JumpZVelocity = 10000.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	Fsm = CreateDefaultSubobject<UAwakenThorFSM>(TEXT("FSM"));

	Mjolnir = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mjolnir"));
	Mjolnir->SetupAttachment(GetMesh());
	Mjolnir->SetCollisionProfileName(TEXT("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tmpMesh(TEXT("/Script/Engine.StaticMesh'/Game/CSW/Asset/Model/weapon/Mjolnir.Mjolnir'"));
	if (tmpMesh.Succeeded())
	{
		Mjolnir->SetStaticMesh(tmpMesh.Object);
	}
	
	MjolnirMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MjolnirMoveComp"));
	MjolnirMoveComp->SetUpdatedComponent(Mjolnir);
	MjolnirMoveComp->ProjectileGravityScale = 0.f;
	MjolnirMoveComp->Velocity = FVector::ZeroVector;

	LEyeLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LEyeLight"));
	LEyeLight->SetupAttachment(GetMesh(), FName("Head"));
	LEyeLight->SetRelativeLocation(FVector(14.000000f,-56.000000f,69.000000f));
	LEyeLight->SetRelativeRotation(FRotator( 0.000000f,90.000000f,0.000000f));
	LEyeLight->SetRelativeScale3D(FVector(4.000000, 4.000000, 4.000000));
	LEyeLight->SetIntensity(100.f);
	LEyeLight->SetIntensityUnits(ELightUnits::Unitless);
	LEyeLight->SetLightColor(FLinearColor(0, 63, 255, 255));
	LEyeLight->SetAttenuationRadius(0.75f);
	
	REyeLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("REyeLight"));
	REyeLight->SetupAttachment(GetMesh(), FName("Head"));
	REyeLight->SetRelativeLocation(FVector(-16.285931f, -57.000000f, 66.931366f));
	REyeLight->SetRelativeRotation(FRotator( 0.000000f,90.000000f,0.000000f));
	REyeLight->SetRelativeScale3D(FVector(4.000000, 4.000000, 4.000000));
	REyeLight->SetIntensity(100.f);
	REyeLight->SetIntensityUnits(ELightUnits::Unitless);
	REyeLight->SetLightColor(FLinearColor(0, 63, 255, 255));
	REyeLight->SetAttenuationRadius(0.75f);

	LMjolnirLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LMjolnirLight"));
	LMjolnirLight->SetupAttachment(Mjolnir);
	LMjolnirLight->SetRelativeLocation(FVector(-43.670495f, -0.000000f, 20.538150f));
	LMjolnirLight->SetIntensity(10000.0f);
	LMjolnirLight->SetIntensityUnits(ELightUnits::Unitless);
	LMjolnirLight->SetLightColor(FLinearColor(0, 63, 255, 255));
	LMjolnirLight->SetAttenuationRadius(30.f);
	
	RMjolnirLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RMjolnirLight"));
	RMjolnirLight->SetupAttachment(Mjolnir);
	RMjolnirLight->SetRelativeLocation(FVector(-43.670495f,0.000000f,-15.538509f));
	RMjolnirLight->SetIntensity(10000.0f);
	RMjolnirLight->SetIntensityUnits(ELightUnits::Unitless);
	RMjolnirLight->SetLightColor(FLinearColor(0, 63, 255, 255));
	RMjolnirLight->SetAttenuationRadius(30.f);

	
	GetCharacterMovement()->GravityScale = 15.f;
	GetCharacterMovement()->MaxAcceleration = 1000000.0f;
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	GetCharacterMovement()->JumpZVelocity = 10000.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AAwakenThor::BeginPlay()
{
	Super::BeginPlay();
	Hp = 10.f;
	Mjolnir->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandSocket"));

}

// Called every frame
void AAwakenThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);
}

// Called to bind functionality to input
void AAwakenThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UProjectileMovementComponent* AAwakenThor::GetMjolnirMoveComp() const
{
	return MjolnirMoveComp;
}

UStaticMeshComponent* AAwakenThor::GetMjolnir() const
{
	return Mjolnir;
}

void AAwakenThor::Move(FVector NewLoc)
{

	FTimerHandle tmpHandle;
	FTimerDelegate tmpDel;
	tmpDel.BindUFunction(this, FName("SetThorLocation"), NewLoc);
	GetWorld()->GetTimerManager().SetTimer(tmpHandle, tmpDel, 1.f, false);
}

void AAwakenThor::ThrowForTeleport(FVector Direction)
{
	Mjolnir->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mjolnir->SetRelativeRotation(Direction.Rotation() - FRotator(-180, 0, 0));
	Direction *= 2000;
	MjolnirMoveComp->Velocity = Direction;
	
}

void AAwakenThor::Teleport(FVector Target)
{
	SetActorLocation(Target);
	Mjolnir->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandSocket"));
	MjolnirMoveComp->Velocity = FVector::ZeroVector;
}

void AAwakenThor::EquipWeapon()
{
	Mjolnir->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandSocket"));
}

void AAwakenThor::DrawWeapon()
{
	Mjolnir->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
}

UAwakenThorFSM* AAwakenThor::getFSM() const
{
	return Fsm;
}

bool AAwakenThor::SetHp(float Damage)
{
	if (Hp <= Damage)
	{
		Hp = 0;
		return true;
	}
	Hp -= Damage;
	return false;
}

float AAwakenThor::GetHpPercent() const
{
	return Hp / MaxHp;
}

void AAwakenThor::SetThorLocation(FVector NewLoc)
{
	SetActorLocation(NewLoc);
	MjolnirMoveComp->Velocity = FVector::ZeroVector;
	Mjolnir->SetRelativeLocation(FVector(0, 60, 20));
}
