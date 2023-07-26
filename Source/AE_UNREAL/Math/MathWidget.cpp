// Fill out your copyright notice in the Description page of Project Settings.


#include "MathWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "MathGameMode.h"

// 보통 헤더를 바꾸면 꺼야 한다.
// 

void UMathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// GameMode를 얻어와부조
	AGameModeBase* GameModePtr = UGameplayStatics::GetGameMode(GetWorld());
	if (nullptr == GameModePtr && GameModePtr->IsValidLowLevel())
	{
		return;
	}

	AMathGameMode* MathGameMode = Cast<AMathGameMode>(GameModePtr);
	MathGameMode->SetWidget(this);

}

void UMathWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CPText = "";

}