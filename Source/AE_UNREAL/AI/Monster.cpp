// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Monster.h"
#include <Global/GlobalGameInstance.h>
#include <Global/Data/MonsterData.h>
#include <AI/AIPlayerCharacter.h>
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
		SetAniState(AIState::IDLE);
	}

	

	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(AIState::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 1500.0f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 200.0f);

	//
	mSpawnPosition = GetActorLocation();
	GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnLocation"), mSpawnPosition);
	GetBlackboardComponent()->SetValueAsInt(TEXT("HP_Monster"), mHealthPoint);

	GetGlobalAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &AMonster::MontageEnd);
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
	if (OtherComp->ComponentHasTag(TEXT("FireBall")))
	{
		//mHealthPoint = GetBlackboardComponent()->GetValueAsInt(TEXT("HP_Monster"));
		mHealthPoint -= 150;
		GetBlackboardComponent()->SetValueAsInt(TEXT("HP_Monster"), mHealthPoint);
		//UE_LOG(LogTemp, Warning, TEXT("%S(%u)> if %d %d"), __FUNCTION__, __LINE__, mHealthPoint, GetBlackboardComponent()->GetValueAsInt(TEXT("HP_Monster")));

		OtherActor->Destroy();

		//GetController()
		//GetBehaviorTree()->SetStateChange(, AIState::DEATH);
		//this->Destroy();
	}
	if (OtherComp->ComponentHasTag(TEXT("WeaponMesh")))
	{
		AAIPlayerCharacter* myCharacter = Cast<AAIPlayerCharacter>(OtherComp->GetAttachmentRootActor());

		if (myCharacter == nullptr && myCharacter->IsValidLowLevel() == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("%S(%u)> AAIPlayerCharacter* myCharacter is wrong"), __FUNCTION__, __LINE__ );
			return;
		}

		if (AIAniState::Attack != myCharacter->GetAniState<AIAniState>())
		{
			UE_LOG(LogTemp, Warning, TEXT("%S(%u)> myCharacter isnt playing Attack"), __FUNCTION__, __LINE__);
			return;
		}
		//OtherComp->GetAttachmentRootActor();
		//mHealthPoint = GetBlackboardComponent()->GetValueAsInt(TEXT("HP_Monster"));
		mHealthPoint -= 130;
		GetBlackboardComponent()->SetValueAsInt(TEXT("HP_Monster"), mHealthPoint);
		//UE_LOG(LogTemp, Warning, TEXT("%S(%u)> if %d %d"), __FUNCTION__, __LINE__, mHealthPoint, GetBlackboardComponent()->GetValueAsInt(TEXT("HP_Monster")));
		//GetBehaviorTree()->SetStateChange(GetBehaviorTree(), AIState::DEATH);
		//this->Destroy();
	}
}

void AMonster::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	TSubclassOf<UAnimInstance> Inst = AMonster::StaticClass();

	// Anim 종료된 몽타주
	if (Anim == GetAnimMontage(AIState::DEATH))
	{
		this->Destroy();
	}

}