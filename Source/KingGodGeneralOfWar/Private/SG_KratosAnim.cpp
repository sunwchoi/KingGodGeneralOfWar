// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"


void USG_KratosAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(("%d"), GetPlayerState()));

}

EPlayerState USG_KratosAnim::GetPlayerState()
{
	AKratos* Owner = Cast<AKratos>(GetOwningActor());
	if (Owner)
	{
		return Owner->State;
	}
	return EPlayerState::Attack;
}
