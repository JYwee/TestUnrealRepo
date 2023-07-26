// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "GameMeshData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct AE_UNREAL_API FGameMeshData : public FTableRowBase
{
	GENERATED_BODY()

	FGameMeshData() {}
	~FGameMeshData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UStaticMesh* Mesh;
};
