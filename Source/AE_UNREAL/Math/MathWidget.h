// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MathWidget.generated.h"

/**
 * 
 */
UCLASS()
class AE_UNREAL_API UMathWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString CPText;

	int Value = 0;

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 다른 액터들이 필요할때마다
	// 이녀석을 아는게 좋지
	// 100개면
	// AMathGameMode* Mode;
	// APlayer*
};
