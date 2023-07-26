// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MathGameMode.generated.h"

// 일반적인 Enum은 UPROPERTY가 붙을수가 없다.
// 언리얼에게 언리얼 블루프린트에서 사용할수 있는 ENUM 이라는 알려줘야 한다.

// 

UENUM(BlueprintType)
enum class OtherState : uint8
{
	Idle,
	LocalTrace,
	Jump,
	PTrace,
	Look, // 순간이동하듯이 
	WorldTrace,
	AngleRot, 
	AngleLook, // 순간이동하듯이 
	AngleAccLook, // 순간이동하듯이 
};

/**
 * 
 */
UCLASS()
class AE_UNREAL_API AMathGameMode : public AGameMode
{
	GENERATED_BODY()

	friend class UMathWidget;

public:

	UFUNCTION(BlueprintCallable)
	void Reset();

	void AddDebugText(const FString& _Text);


protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;


private:
	// private이기 때문에 블루프린트에서 손쉽게 바꿀수 없는데
	// meta=(AllowPrivateAccess = "true") private이어도 블루프린트에서 사용하겠다.
	UPROPERTY(Category = "OtherActor", EditAnywhere, BlueprintReadWrite, DisplayName = "OtherActor 상태", meta = (AllowPrivateAccess = "true"))
	OtherState OtherActorState = OtherState::Idle;

	UPROPERTY(Category = "OtherActor", EditAnywhere, BlueprintReadWrite, DisplayName = "OtherActor TestValue", meta = (AllowPrivateAccess = "true"))
	int ValueTest = 0;

	// 자기들만의 std::string을 따로 만들었다.
	UPROPERTY(Category = "OtherActor", EditAnywhere, BlueprintReadWrite, DisplayName = "OtherActor TestValue", meta = (AllowPrivateAccess = "true"))
	FString AngleText = TEXT("Angle");

	FVector UpVector = { 5000, 0, 0 };
	FVector OriPos = FVector::ZeroVector;
	FVector PTraceDir = FVector::ZeroVector;
	AActor* MainActor = nullptr;
	AActor* OtherActor = nullptr;

	class UMathWidget* DebugWidget = nullptr;

	FORCEINLINE void SetWidget(UMathWidget* _DebugWidget)
	{
		DebugWidget = _DebugWidget;
	}
};
