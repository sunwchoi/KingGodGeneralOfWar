// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"


void USG_KratosAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	UpdatePlayerState();

}

void USG_KratosAnim::UpdatePlayerState()
{
	AKratos* Player = Cast<AKratos>(GetOwningActor());
	if (Player)
	{
		FVector velocity = Player->GetVelocity();
		FVector forwardVector = Player->GetActorForwardVector();

		PlayerState = Player->State;
		Speed = FVector::DotProduct(forwardVector, velocity);
		Direction = FVector::DotProduct(Player->GetActorRightVector(), velocity);
	}
}
