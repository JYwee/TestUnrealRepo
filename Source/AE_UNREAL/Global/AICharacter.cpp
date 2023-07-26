// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <AI/AICon.h>

UBlackboardComponent* AAICharacter::GetBlackboardComponent()
{
	if (nullptr == BlackboardComponent)
	{
		AAICon* AiCon = GetController<AAICon>();

		if (nullptr == AiCon)
		{
			return nullptr;
		}

		BlackboardComponent = AiCon->GetBlackboardComponent();
	}

	return BlackboardComponent;
}