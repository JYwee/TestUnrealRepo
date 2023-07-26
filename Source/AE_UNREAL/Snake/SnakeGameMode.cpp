// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SnakePart.h"

void ASnakeGameMode::BeginPlay()
{
	Super::BeginPlay();
	// [][][][][]
	// []      []
	// []      []
	// []      []
	// [][][][][]

	RandomCreate.GenerateNewSeed();

}

void ASnakeGameMode::CreateOutWall(int _Y, int _Z, FVector _TileSize, TSubclassOf<AActor> _WallType)
{
	// 대부부느이 언리얼 엔진의 컴포넌트들이 들고 있는 
	// 3차원에서의 위치값을 세팅해주고 처리하기 위해서 주고받는 구조체 입니다.
	// F가 붙어있으면 구조체
	// FTransform Trans;
	//TPersistentVectorRegisterType<T> Rotation; 회전
	//TPersistentVectorRegisterType<T> Translation; 위치
	//TPersistentVectorRegisterType<T> Scale3D; 크기

	// s멀티바이트고 
	UE_LOG(LogTemp, Log, TEXT("%S(%u)> MapCreate Start"), __FUNCTION__, __LINE__);

	TileCount.Y = _Y;
	TileCount.Z = _Z;

	TileSize = _TileSize;


	// 
	AllMapActor.SetNumZeroed(TileCount.Z);
	// Num() => size()
	for (size_t z = 0; z < AllMapActor.Num(); z++)
	{
		AllMapActor[z].SetNum(TileCount.Y);
	}
	// Wall::StaticClass() <= 지금 내가 생성하려는 액터가 뭔지를 알려줍니다.
	// 이 내부에서 블루프린트 wall을 알수 있어야 한다.
	for (size_t i = 0; i < TileCount.Y; i++)
	{
		FTransform Trans;
		Trans.SetLocation({ 0, TileSize.Y * static_cast<float>(i), 0.0f });
		AActor* SpawnedActor0 = GetWorld()->SpawnActor<AActor>(_WallType, Trans);

		Trans.SetLocation({ 0, TileSize.Y * static_cast<float>(i), TileSize.Z * static_cast<float>(TileCount.Z - 1) });
		AActor* SpawnedActor1 = GetWorld()->SpawnActor<AActor>(_WallType, Trans);
	}

	for (size_t i = 0; i < TileCount.Z; i++)
	{
		FTransform Trans;
		Trans.SetLocation({ 0, 0, TileSize.Z * i });
		AActor* SpawnedActor0 = GetWorld()->SpawnActor<AActor>(_WallType, Trans);

		Trans.SetLocation({ 0, TileSize.Y * static_cast<float>(TileCount.Y - 1), TileSize.Z * i });
		AActor* SpawnedActor1 = GetWorld()->SpawnActor<AActor>(_WallType, Trans);
	}


}

void ASnakeGameMode::SetPart(AActor* _Part)
{
	// 배열에서의 x y가 될수는 없다.
	//          200                       100

	FVector Pos = _Part->GetActorLocation();

	int Y = Pos.Y / TileSize.Y;
	int Z = Pos.Z / TileSize.Z;

	SetPart(Y, Z, _Part);

	// SetPart(, _Part);
}

void ASnakeGameMode::SetPart(int _Y, int _Z, AActor* _Part)
{
	if (0 > _Y)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (0 >= Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return;
	}

	if (0 > _Z)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (0 >= Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return;
	}

	if (_Y >= TileCount.Y)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (Y >= TileCount.Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return;
	}

	if (_Z >= TileCount.Z)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (Z >= TileCount.Z) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return;
	}

	AllMapActor[_Z][_Y] = _Part;
}

void ASnakeGameMode::Tick(float _Delta)
{
	Super::Tick(_Delta);

	// 1. 화면의 너비는 게임모드가 알아.
	TileCount;

	// 2. 화면내에 존재하는 모든 SnakePart를 상속받은 액터들을 알고 싶어.
	// 쉽게 해결하는 방법은 느리고 위험부담이 있다.

	// 여러번 실행하는건 확실히 잘못생각하고 있는거죠
	// 모두 얻어온 다음 구분하는게 당연히 좀더 속력을 생각한 코딩일 것이다.
	TArray<AActor*> AllSnakeActor;
		//                                   이레벨에 있는  이 태그를 가진 액터를 AllSnakeActor에 담아줘
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "SnakePart", AllSnakeActor);
	// 빠르게 만들고 싶다
	// => 오브젝트가 그렇게 많나?
	// 1000개 이상인가? 100개 이하일거야.
	// 그런 상황에서 뭘해도 느려지만 무슨짓을 

	// Fname을 사용한다.
	// "aaaaaa" "aaaaaab"
	// unreal은 그 최적화를 위해서 FName 자료형을 만들었다.
	// FName은 "fasdhjkfasdhjkfasdjklfaskjlfasdjkl" => 223 해쉬 방식이라고 합니다.
	// FName은 내부에서 문자열을 정수 1개 짜리로 바꿔준다.
	// FName 비교는 정수 비교입니다.
	// 내부에 액터가 100만개 

	AllMapClear();
	
	for (size_t i = 0; i < AllSnakeActor.Num(); i++)
	{
		SetPart(AllSnakeActor[i]);
	}

	if (nullptr == BodyType)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == BodyType)"), __FUNCTION__, __LINE__);
		return;
	}

	if (nullptr == CurBody && nullptr != BodyType)
	{

		TArray<FIntVector> EmptyPoint;

		for (int z = 0; z < AllMapActor.Num(); z++)
		{
			for (int y = 0; y < AllMapActor[z].Num(); y++)
			{
				if (nullptr != AllMapActor[z][y])
				{
					continue;
				}

				EmptyPoint.Add({0, y, z});
			}
		}

		// 내부에서 seed를 시간으로 처리해줘야 진짜 랜덤한 위치를 사용할수 있다.

		// 0 63
		// 언리얼은 max까지 잘 나옵니다.
		int RandomIndex = RandomCreate.RandRange(0, EmptyPoint.Num() - 1);

		FIntVector PointVector = EmptyPoint[RandomIndex];

		FTransform Trans;

		Trans.SetLocation({0.0f, PointVector.Y * TileSize.Y, PointVector.Z * TileSize.Z });


		CurBody = GetWorld()->SpawnActor<AActor>(BodyType, Trans);
	}
}

void ASnakeGameMode::AllMapClear()
{
	for (size_t z = 0; z < AllMapActor.Num(); z++)
	{
		for (size_t y = 0; y < AllMapActor[z].Num(); y++)
		{
			AllMapActor[z][y] = nullptr;
		}
	}
}

bool ASnakeGameMode::IsPart(FVector _Pos, FName _Tag)
{
	int Y = _Pos.Y / TileSize.Y;
	int Z = _Pos.Z / TileSize.Z;

	AllMapActor;

	return IsPart(Y, Z, _Tag);
}

bool ASnakeGameMode::IsPart(int _Y, int _Z, FName _Tag)
{
	if (0 > _Y)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (0 >= Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return false;
	}

	if (0 > _Z)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (0 >= Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return false;
	}

	if (_Y >= TileCount.Y)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (Y >= TileCount.Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return false;
	}

	if (_Z >= TileCount.Z)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (Z >= TileCount.Z) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return false;
	}

	if (nullptr == AllMapActor[_Z][_Y])
	{
		return false;
	}

	return AllMapActor[_Z][_Y]->ActorHasTag(_Tag);
}

AActor* ASnakeGameMode::GetPart(FVector _Pos, FName _Tag) 
{
	int Y = _Pos.Y / TileSize.Y;
	int Z = _Pos.Z / TileSize.Z;

	return GetPart(Y, Z, _Tag);
}

AActor* ASnakeGameMode::GetPart(int _Y, int _Z, FName _Tag) 
{
	if (0 > _Y)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (0 >= Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return nullptr;
	}

	if (0 > _Z)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (0 >= Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return nullptr;
	}

	if (_Y >= TileCount.Y)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (Y >= TileCount.Y) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return nullptr;
	}

	if (_Z >= TileCount.Z)
	{
		UE_LOG(LogTemp, Log, TEXT("%S(%u)> if (Z >= TileCount.Z) [Y : %d][Z : %d]"), __FUNCTION__, __LINE__, _Y, _Z);
		return nullptr;
	}

	if (nullptr == AllMapActor[_Z][_Y])
	{
		return nullptr;
	}

	if (false == AllMapActor[_Z][_Y]->ActorHasTag(_Tag))
	{
		return nullptr;
	}

	return AllMapActor[_Z][_Y];
}

void ASnakeGameMode::CurBodyReset()
{
	CurBody = nullptr;
}