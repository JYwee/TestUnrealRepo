// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakePart.h"
#include "SnakeHead.generated.h"

UCLASS()
class AE_UNREAL_API ASnakeHead : public ASnakePart
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 언리얼의 블루프린트가 실행할수 있게 이 함수를 등록시켜줘야 합니다.
	// 블루프린트가 이 c++함수를 알수있게 만들어줘야 한다.
	// 함수의 경우에는 

	// 이 UFUNCTION 언리얼이 인식하게 해준것 뿐이고
	// 이제부터 이 함수가 어떻게 쓰일건지를 () 넣어줘야 합니다.



	UFUNCTION(BlueprintCallable)
	void LeftMove(float _Value);

	UFUNCTION(BlueprintCallable)
	void RightMove(float _Value);

	UFUNCTION(BlueprintCallable)
	void UpMove(float _Value);

	UFUNCTION(BlueprintCallable)
	void DownMove(float _Value);

	UFUNCTION(BlueprintCallable)
		void LeftAction();

	UFUNCTION(BlueprintCallable)
		void RightAction();

	UFUNCTION(BlueprintCallable)
		void UpAction();

	UFUNCTION(BlueprintCallable)
		void DownAction();

	void MoveLogic(const FVector& _MoveVector);

};
