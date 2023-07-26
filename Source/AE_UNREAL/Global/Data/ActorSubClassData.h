// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ActorSubClassData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct AE_UNREAL_API FActorSubClassData : public FTableRowBase
{
	GENERATED_BODY()

	FActorSubClassData() {}
	~FActorSubClassData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AActor> Objects;
};
