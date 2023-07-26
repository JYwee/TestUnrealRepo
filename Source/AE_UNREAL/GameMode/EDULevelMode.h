// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EDULevelMode.generated.h"

/**
 * 
 */
UCLASS()
class AE_UNREAL_API AEDULevelMode : public AGameMode
{
	GENERATED_BODY()

	AEDULevelMode();

	void BeginPlay() override;
};
