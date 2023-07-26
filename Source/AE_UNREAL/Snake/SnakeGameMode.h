// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SnakeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AE_UNREAL_API ASnakeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// 시작하면서 맵의 외곽 벽을 만들고 맵을 정의하는 함수
	// UFUNCTION 이거 블루프린트에서 쓸 함수다.
	// BlueprintCallable => 블루프린트에서 호출도 할거다.
	// 내가 C++에서 만든 함수를 언리얼 에디터에서 사용하기 위한 방법
	UFUNCTION(BlueprintCallable)
	void CreateOutWall(int _Y, int _Z, FVector _TileSize, TSubclassOf<AActor> _WallType);

	FORCEINLINE FVector GetTileSize()
	{
		return TileSize;
	}

	FORCEINLINE FIntVector3 GetCount()
	{
		return TileCount;
	}

	void SetPart(class AActor* _Part);

	void SetPart(int _Y, int _Z, class AActor* _Part);

	bool IsPart(FVector _Pos, FName _Tag);

	bool IsPart(int _Y, int _Z, FName _Tag);

	AActor* GetPart(FVector _Pos, FName _Tag);

	AActor* GetPart(int _Y, int _Z, FName _Tag);

	void CurBodyReset();

protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

private:
	FVector TileSize;
	FIntVector3 TileCount;

	// std::vector<AActor*>

	// WeWeWeWeWeWeWeWeWe
	// We[][][]HE[][][]We
	// We[][][]HE[][][]We
	// We[][][]HE[][][]We
	// We[][][]HE[][][]We
	// We[][][][][][][]We
	// We[][][][][][][]We
	// WeWeWeWeWeWeWeWeWe
	// AllMapActor.Clear();

	TArray<TArray<class AActor*>> AllMapActor;

	void AllMapClear();

	AActor* CurBody = nullptr;

	FRandomStream RandomCreate = FRandomStream();


	// 이번에는 변수를 블루프린트에서 세팅하고 변경하고 싶다.
	UPROPERTY(Category = "GameModeValue", EditAnywhere, DisplayName = "바디 오브젝트 클래스")
		TSubclassOf<AActor> BodyType;

	UPROPERTY(Category = "GameModeValue", EditAnywhere, DisplayName = "그냥 테스트용 변수")
		int Value;
};
