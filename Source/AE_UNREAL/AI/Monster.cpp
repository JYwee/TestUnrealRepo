// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Monster.h"
#include <Global/GlobalGameInstance.h>
#include <Global/Data/MonsterData.h>
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


void AMonster::BeginPlay()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurMonsterData = Inst->GetMonsterData(DataName);

		SetAllAnimation(CurMonsterData->MapAnimation);
		SetAniState(AIState::DEATH);
	}

	

	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(AIState::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 1500.0f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 200.0f);

	//
	mSpawnPosition = GetActorLocation();
	GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnLocation"), mSpawnPosition);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMonster::BeginOverLap);
}

void AMonster::BeginOverLap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("FireBall")))
	{
		GetBehaviorTree()->SetStateChange(GetBehaviorTree(), AIState::DEATH);
		this->Destroy();
	}
	if (OtherComp->ComponentHasTag(TEXT("WeaponMesh")))
	{
		GetBehaviorTree()->SetStateChange(GetBehaviorTree(), AIState::DEATH);
		this->Destroy();
	}
}