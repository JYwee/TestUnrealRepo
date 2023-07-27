// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/AICharacter.h"
#include <Global/GlobalEnums.h>
#include <Global/Data/MonsterData.h>
#include "Monster.generated.h"

/**
 * 
 */
UCLASS()
class AE_UNREAL_API AMonster : public AAICharacter
{
	GENERATED_BODY()

	const struct FMonsterData* CurMonsterData;

	void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Monster")
	FORCEINLINE FVector GetSpawnPosition() const{
		return mSpawnPosition;
	}

	UFUNCTION(BlueprintCallable, Category = "Monster")
		FORCEINLINE void SetSpawnPosition(FVector pos) {
		mSpawnPosition = pos;
	}

	UFUNCTION(BlueprintCallable, Category = MonsterCollision)
	void BeginOverLap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName DataName = "NONE";

	UPROPERTY(Category = "Monster", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector mSpawnPosition;

};
