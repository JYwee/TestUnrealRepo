// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePawn.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeGameMode.h"

// Sets default values
ASnakePawn::ASnakePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnakePawn::BeginPlay()
{
	Super::BeginPlay();
	
	// 요걸 이용하는것이 보통 일반적이다.
	// 언리얼에서 GameInstacne가 일반적으로 이런 값들을 주고받는 용도로 지정된 녀석이다.
	// GetGameInstance()

	// 지금 내 월드의 GameMode를 내놔.
	// 내월드 내 레벨을 말합니다.
	// 내 월드 내 레벨에 세팅된 GameMode를 알려면 당연히 내월드 내 Level을 알아야 

	AGameModeBase* GameModePtr = UGameplayStatics::GetGameMode(GetWorld());
	// 다운 캐스트를 해야합니다.
	if (nullptr == GameModePtr)
	{
		// 이 TEXT에 대해서도 설명하자면
		// 언리얼에서는 유니코드 멀티바이트를 구분하지 않는다.
		// 어떤 국가코드와 언리얼엔진이 알아서 찾아준다.
		// 문자열은 여러개의 인코딩이 있는에 그 인코딩을 알아서 자신들이 처리하겠다는 의미로
		// TEXT() 알아서 유니코드건 멀티바이트건 뭐건 알아서 잘 처리하겠다.
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == GameMode)"), __FUNCTION__, __LINE__);
		return;
	}

	// 언리얼의 라이브러리는 이녀석이 단순히 nullptr이 아니라는 것 만으로는 안정성을 확인할수
	// 없다고 생각해서 무조건 UObject안에 맴버함수인
	// 내가 사용하려는 언리얼의 UOBject를 상속받은 포인터가 정상적인 포인터인지 확인하려면
	// 단순히 nullptr만으로 안되고 IsValidLowLevel 함수를 사용해서 언리얼이 제대로 관리해주고 있는
	// 포인터인지 알아낼수가 있다.
	if (false == GameModePtr->IsValidLowLevel())
	{
		// 언리얼의 검사상 이포인터는 위험할 가능성이 있다는것이다.
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == GameMode)"), __FUNCTION__, __LINE__);
		return;
	}

	// 다운캐스팅
	ASnakeGameMode* SnakeGameMode = Cast<ASnakeGameMode>(GameModePtr);

	if (nullptr == SnakeGameMode && false == SnakeGameMode->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == SnakeGameMode && false == SnakeGameMode->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return;
	}

	FIntVector3 Count = SnakeGameMode->GetCount();
	FVector Size = SnakeGameMode->GetTileSize();
	// /가 사칙연산중 가장 느린 연산입니다.
	// /2 할바에는 *0.5f

	FVector Center = {-1000.0f, Size.Y * Count.Y * 0.5f, Size.Z * Count.Z * 0.5f };

	SetActorLocation(Center);
}

// Called every frame
void ASnakePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 나의 위치에서 더해서 처리하는 함수
	// AddActorLocalOffset
	
	// 현재 나의 위치가 어떻게 되어있건 이 위치로 이동해라

}

// Called to bind functionality to input
void ASnakePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void ASnakePawn::HeadPositionToCenter()
{

	AGameModeBase* GameModePtr = UGameplayStatics::GetGameMode(GetWorld());
	if (nullptr == GameModePtr)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == GameMode)"), __FUNCTION__, __LINE__);
		return;
	}

	if (false == GameModePtr->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == GameMode)"), __FUNCTION__, __LINE__);
		return;
	}

	ASnakeGameMode* SnakeGameMode = Cast<ASnakeGameMode>(GameModePtr);

	if (nullptr == SnakeGameMode && false == SnakeGameMode->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == SnakeGameMode && false == SnakeGameMode->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return;
	}

	FIntVector3 Count = SnakeGameMode->GetCount();
	FIntVector3 CenterCount;
	// 정수는 0.5가 없다.
	CenterCount.Y = (Count.Y) / 2;
	CenterCount.Z = (Count.Z) / 2;
	FVector Size = SnakeGameMode->GetTileSize();

	FVector Center = { 0.0f, Size.Y * CenterCount.Y, Size.Z * CenterCount.Z};

	Head->SetActorLocation(Center);
}
