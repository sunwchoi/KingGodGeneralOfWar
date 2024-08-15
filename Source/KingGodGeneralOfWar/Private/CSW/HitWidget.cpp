// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/HitWidget.h"

void UHitWidget::PlayHitAnimation()
{
	if (HitAnim)
		PlayAnimation(HitAnim);
}
