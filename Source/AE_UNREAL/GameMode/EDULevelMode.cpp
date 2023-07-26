// Fill out your copyright notice in the Description page of Project Settings.


#include "EDULevelMode.h"

AEDULevelMode::AEDULevelMode() 
{

}

void AEDULevelMode::BeginPlay() 
{
	// 언리얼이 C++ 컴파일러를 조금 수정해놔서. 
	// AGameMode::BeginPlay();
	Super::BeginPlay();

	// 언리얼의 로그를 띄우는 방식
	// UE_LOG(LogTemp, Error, TEXT("게임모드가 만들어졌습니다."));
	// 
	
	UE_LOG(LogTemp, Log, TEXT("Test Log")); // 흰색
	UE_LOG(LogTemp, Warning, TEXT("Test Error")); // 노란색
	UE_LOG(LogTemp, Error, TEXT("Test Error")); // 빨간색
	// UE_LOG(LogTemp, Fatal, TEXT("Test Error")); // 진짜 엔진을 정지시킨다.


	// 뭔가를 했다면 중단점을 걸어서 확인하고 싶을 것이다.
	
}