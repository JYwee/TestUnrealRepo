// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake/SnakeHead.h"
#include "SnakeGameMode.h"
#include "../Global/GlobalGameInstance.h"

// Sets default values
ASnakeHead::ASnakeHead()
{
	// 언리얼에서 프레임마다 Tick을 돌려야 하는 액터는
	// PrimaryActorTick.bCanEverTick = true; 로 만들어줘야 Tick을 돌릴수가 있습니다.
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 생성자에서 보통 설정해주는게 일반적인데. 
	// 틱을 돌릴거냐 말거냐
	PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;


}

// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeHead::LeftMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}

	AddActorLocalOffset(FVector::LeftVector);
}


void ASnakeHead::RightMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}

	AddActorLocalOffset(FVector::RightVector);
}
void ASnakeHead::UpMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}
	AddActorLocalOffset(FVector::UpVector);
}
void ASnakeHead::DownMove(float _Value)
{
	if (0 == _Value)
	{
		return;
	}
	AddActorLocalOffset(FVector::DownVector);
}

void ASnakeHead::MoveLogic(const FVector& _MoveVector)
{
	if (true == GetSnakeGameMode()->IsPart(GetActorLocation() + _MoveVector, "Wall"))
	{
		return;
	}

	{
		FVector HeadPos = GetActorLocation() + _MoveVector;
		FIntVector HeadIndex = FIntVector(HeadPos.X, HeadPos.Y, HeadPos.Z);

		ASnakePart* CurNextPart = GetNext();
		while (nullptr != CurNextPart)
		{
			// 고려해야할게 뭐가 있을까?
			// 실수는 0이 아니면 비교가 오차로 인해서 실패할때가 있다.
			FVector BodyPos = CurNextPart->GetActorLocation();

			// 이럴수 있는 이유가?????
			// deltatime으로 이동하고 있지 않기 때문에
			// 정수로 변경해서 비교하면 비교가 가능하다는 
			FIntVector BodyIndex = FIntVector(BodyPos.X, BodyPos.Y, BodyPos.Z);

			if (HeadIndex == BodyIndex)
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> Game End"), __FUNCTION__, __LINE__);
				return;
			}

			CurNextPart = CurNextPart->GetNext();
		}
	}

	SetPrevPos(GetActorLocation());

	AddActorLocalOffset(_MoveVector);

	{
		if (nullptr != GetNext())
		{
			ASnakePart* CurNextPart = GetNext();

			while (nullptr != CurNextPart)
			{
				CurNextPart->SetPrevPos(CurNextPart->GetActorLocation());
				CurNextPart->SetActorLocation(CurNextPart->GetPrev()->GetPrevPos());
				CurNextPart = CurNextPart->GetNext();
			}
		}
	}


	// 여기에 Body가 있는지 확인해야 한다.
	if (true == GetSnakeGameMode()->IsPart(GetActorLocation(), "Body"))
	{
		AActor* Body = GetSnakeGameMode()->GetPart(GetActorLocation(), "Body");
		ASnakePart* BodyPart = Cast<ASnakePart>(Body);

		if (nullptr == BodyPart)
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Part)"), __FUNCTION__, __LINE__);
			return;
		}

		ASnakePart* LastPart = LastNextPart();
		// 옮기자마자 게임모드에 등록되어야 한다.
		BodyPart->SetActorLocation(LastPart->GetPrevPos());
		LastPart->SetNext(BodyPart);
		BodyPart->SetPrev(LastPart);
		GetSnakeGameMode()->CurBodyReset();
	}
}

void ASnakeHead::LeftAction()
{
	FVector MovePos = FVector::LeftVector * 100.0f;
	MoveLogic(MovePos);
}
void ASnakeHead::RightAction()
{
	FVector MovePos = FVector::RightVector * 100.0f;
	MoveLogic(MovePos);
}
void ASnakeHead::UpAction()
{
	FVector MovePos = FVector::UpVector * 100.0f;
	MoveLogic(MovePos);
}
void ASnakeHead::DownAction()
{
	FVector MovePos = FVector::DownVector * 100.0f;
	MoveLogic(MovePos);
}