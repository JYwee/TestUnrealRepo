// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathMainActor.generated.h"

UCLASS()
class AE_UNREAL_API AMathMainActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMathMainActor();

	UFUNCTION(BlueprintCallable)
		void LeftMove(float _Value);

	UFUNCTION(BlueprintCallable)
		void RightMove(float _Value);

	UFUNCTION(BlueprintCallable)
		void UpMove(float _Value);

	UFUNCTION(BlueprintCallable)
		void DownMove(float _Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:	
	float Speed = 500.0f;
	// Called every frame

};
