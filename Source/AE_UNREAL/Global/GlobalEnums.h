// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 애니메이션에 집중할거냐
// 캐릭터에 집중할 거냐?

// 몬스터와 플레이어가 정말 모든 애니메이션 상태에 대해서 공유가 가능하냐를 생각해보고
// 불가능하다면 Enum은 사용이 안된다.


UENUM(BlueprintType)
enum class AIState : uint8
{
	NONE UMETA(DisplayName = "사용안함"),
	IDLE UMETA(DisplayName = "대기"),
	MOVE UMETA(DisplayName = "이동"),
	RETURN UMETA(DisplayName = "복귀"),
	PATROL UMETA(DisplayName = "순찰"),
	ATTACK UMETA(DisplayName = "공격"),
	DEATH UMETA(DisplayName = "죽음"),
};

