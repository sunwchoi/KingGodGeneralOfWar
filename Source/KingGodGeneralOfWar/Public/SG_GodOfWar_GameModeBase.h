// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SG_GodOfWar_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API ASG_GodOfWar_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASG_GodOfWar_GameModeBase();

public:
	virtual void BeginPlay() override;

	//임시로 UI 넣어둠
	UPROPERTY(EditAnywhere)
	TSubclassOf <UUserWidget> BDThorFadeOutFactory; //페이드 아웃 UI 클래스 호출

	UPROPERTY()
	class UBDthorFadeOut* BDThorFadeOutUI;

	UFUNCTION()
	void ThorFadeOut();
	
};
