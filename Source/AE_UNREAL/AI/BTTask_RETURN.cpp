// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RETURN.h"
#include <Global/ARGlobal.h>
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_RETURN::UBTTask_RETURN()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}


EBTNodeResult::Type UBTTask_RETURN::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::RETURN);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 800.0f;
	}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_RETURN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);

	FVector firstLocation = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("SpawnLocation"));

	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	// 혹시라도 z축이 있을 가능성을 없애는게 보통입니다.
	firstLocation.Z = 0.0f;
	ThisPos.Z = 0.0f;

	

	FVector Dir = firstLocation - ThisPos;
	Dir.Normalize();

	FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
	OtherForward.Normalize();

	FVector Cross = FVector::CrossProduct(OtherForward, Dir);

	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = OtherForward.Rotation().Yaw;

	if (FMath::Abs(Angle0 - Angle1) >= 10.0f)
	{
		FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DelataSeconds });
		GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
	}
	else {
		FRotator Rot = Dir.Rotation();
		GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
	}

	GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);

	Dir = firstLocation - ThisPos;
	if (1.0f >= Dir.Size())
	{
		SetStateChange(OwnerComp, AIState::IDLE);
		return;
	}
}
	

