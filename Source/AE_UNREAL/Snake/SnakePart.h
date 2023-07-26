// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakePart.generated.h"
// Part.generated.h ¹«Á¶°Ç 

UCLASS()
class AE_UNREAL_API ASnakePart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakePart();

	class ASnakeGameMode* GetSnakeGameMode();

	FORCEINLINE FVector GetPrevPos() 
	{
		return PrevPos;
	}

	FORCEINLINE void SetPrevPos(const FVector& _Pos)
	{
		PrevPos = _Pos;
	}

	FORCEINLINE ASnakePart* GetNext()
	{
		return Next;
	}
	FORCEINLINE ASnakePart* GetPrev()
	{
		return Prev;
	}

	FORCEINLINE void SetNext(ASnakePart* _Next)
	{
		Next = _Next;
	}
	FORCEINLINE void SetPrev(ASnakePart* _Prev)
	{
		Prev = _Prev;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ASnakePart* LastNextPart();

private:	
	FVector PrevPos;
	ASnakePart* Next;
	ASnakePart* Prev;
};
