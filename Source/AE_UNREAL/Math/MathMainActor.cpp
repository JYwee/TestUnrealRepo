// Fill out your copyright notice in the Description page of Project Settings.


#include "MathMainActor.h"
#include "Kismet/GameplayStatics.h"
#include "MathGameMode.h"

// Sets default values
AMathMainActor::AMathMainActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMathMainActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMathMainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// GameMode를 얻어와부조
	AGameModeBase* GameModePtr = UGameplayStatics::GetGameMode(GetWorld());
	if (nullptr == GameModePtr && GameModePtr->IsValidLowLevel())
	{
		return;
	}

	AMathGameMode* MathGameMode = Cast<AMathGameMode>(GameModePtr);

	// 함수로 만드는걸 좋아합니다.
	
	FString Text = "PlayerPos : ";

	FVector Pos = GetActorLocation();
	Text += Pos.ToString();

	MathGameMode->AddDebugText(Text);

	// MathGameMode->DebugWidget->CPText += "fasdfasd"
}


void AMathMainActor::LeftMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}

	AddActorLocalOffset(FVector::LeftVector * GetWorld()->DeltaTimeSeconds * Speed);
}


void AMathMainActor::RightMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}

	AddActorLocalOffset(FVector::RightVector * GetWorld()->DeltaTimeSeconds * Speed);
}
void AMathMainActor::UpMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}
	AddActorLocalOffset(FVector::ForwardVector * GetWorld()->DeltaTimeSeconds * Speed);
}

void AMathMainActor::DownMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}
	AddActorLocalOffset(FVector::BackwardVector * GetWorld()->DeltaTimeSeconds * Speed);
}