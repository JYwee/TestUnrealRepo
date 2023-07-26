// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GlobalDebugUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AE_UNREAL_API UGlobalDebugUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AddDebugText(const FString& _Text)
	{
		DebugTextTick += _Text + TEXT("\n");
	}

	UPROPERTY(BlueprintReadWrite)
	FString DebugTextTick;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
