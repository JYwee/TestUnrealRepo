// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSEnums.h"
#include "TPSAnimInstance.generated.h"



/**
 * 
 */
UCLASS()
class AE_UNREAL_API UTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "GameModeValue", EditAnywhere, BlueprintReadWrite)
	TPSAniState AniState = TPSAniState::Idle;

	TMap<TPSAniState, class UAnimMontage*> AllAnimations;

	UFUNCTION()
	void MontageEnd(UAnimMontage* Anim, bool _Inter);

	// 애님인스턴용 Tick과 BeginPlay

protected:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float _DeltaTime) override;
};
