// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "GameMainData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct AE_UNREAL_API FGameMainData : public FTableRowBase
{
	GENERATED_BODY()

	FGameMainData() {}
	~FGameMainData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FString> TPSPlayerStartWeapon;
};
