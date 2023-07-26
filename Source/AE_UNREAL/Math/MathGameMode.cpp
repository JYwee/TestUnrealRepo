// Fill out your copyright notice in the Description page of Project Settings.


#include "MathGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MathWidget.h"

void AMathGameMode::AddDebugText(const FString& _Text)
{
	DebugWidget->CPText += _Text + TEXT("\n");
}

void AMathGameMode::BeginPlay()
{
	{
		TArray<AActor*> AllActor;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Main", AllActor);

		if (1 != AllActor.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> Main Actor Is nullptr"), __FUNCTION__, __LINE__);
			return;
		}

		MainActor = AllActor[0];
	}

	// 어떤 클래스를 가든 
	// 레벨에 속한 클래스라면 
	GetGameInstance();

	{
		// 한번에 완벽하게 치려고하지 않는다. 
		// 형태에 가까운 코드를 치고
		// 예외처리는 엄청 신경써서 해준다.
		// 예외에 걸리면 내가 뭘잘못 했는지 알아내게되고
		// 수정해 나가면서 완벽한 코드가 되게 된다.
		
		// 한번에 완벽한 코드를 치려는 사람들의 문제점
		// 그냥 아무것도 안하는것처럼 보인다.
		// 이 스타일의 사람들은 보통 재능이 어마어마합니다.
		// 10분만에 생각완료 20분만에 완벽한 코드 다침.
		// 재능이 없다. => 코드를 그냥 안치고 있다.
		
		// 문제를 쪼개서 하나하나를 대충쳐
		// 이게 동작안하는 상황을 예외를 잘 걸어놓는다.
		// 한발자국도 못나가는 사람이 있고
		// 자신이 무슨 코드를 생각했는지 나

		TArray<AActor*> AllActor;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Other", AllActor);

		if (1 != AllActor.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> Main Actor Is nullptr"), __FUNCTION__, __LINE__);
			return;
		}

		OtherActor = AllActor[0];

		OriPos = OtherActor->GetActorLocation();
	}

}

void AMathGameMode::Reset()
{
	OtherActorState = OtherState::Idle;
	OtherActor->SetActorLocation(OriPos);
}

void AMathGameMode::Tick(float _Delta) 
{
	if (nullptr == MainActor || nullptr == OtherActor)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == MainActor || nullptr == OtherActor)"), __FUNCTION__, __LINE__);
		return;
	}

	switch (OtherActorState)
	{
	case OtherState::Idle:
	{
		break;
	}
	case OtherState::LocalTrace:
	{
		// 방향을 구하고
		FVector Dir = MainActor->GetActorLocation() - OtherActor->GetActorLocation();
		// 방향을 단위벡터로 만들고
		Dir.Normalize();
		// 이런짓을 하면 절대로 안되.
		// 그냥 1인벡터잖아요? 
		// 시간이 적용안됐으므로 컴퓨터의 성능마다 다르게 움직이게 된다.
		// 이렇게 해주고 여기에 내가 생각하는 속력을 넣어서 맞춰줘야 합니다.
		//                                  방향                시간    속도

		// 이런 이동은 하지만 등속 운동입니다.
		// 로컬은 자기자신이 중심이 되서 좌표계를 만드는데
		// 자신이 회전하게 되면 좌표계도 같이 회전을 하게 될겁니다.
		OtherActor->AddActorLocalOffset(Dir * _Delta * 2000.0f);
		break;
	}
	case OtherState::WorldTrace:
	{
		// 방향을 구하고
		FVector Dir = MainActor->GetActorLocation() - OtherActor->GetActorLocation();
		Dir.Normalize();
		// 월드는 말그대로 월드 좌표계에서의 덧셈을 하고
		OtherActor->AddActorWorldOffset(Dir * _Delta * 500.0f);
		break;
	}
	case OtherState::Jump:
	{
		// 이미 힘을 가지고 이는 거라 시간만 적용되면 됩니다.
		// 언리얼 엔진에서 지역스태틱은 엔진이 꺼져도 초기화가 되지 않는다.
		// 즉 이미 어마어마하게 다운된 값이 그대로 유지가 된다.
		// static은 다 그렇게 처리가 되므로 일반적인 static 맴버변수는 주의해서 사용해야 한다.
		// 왜? 실행되고 난 이후에 다시 초기화가 되지 않기 때문이다.
		// static FVector UpVector = {5000, 0, 0};
		FVector Gravity = { -3000, 0, 0 };
		UpVector += Gravity * _Delta;

		FVector LastVector = UpVector;
		OtherActor->AddActorWorldOffset(LastVector * _Delta);
		break;
	}
	case OtherState::PTrace:
	{
		FVector Dir = MainActor->GetActorLocation() - OtherActor->GetActorLocation();
		Dir.Normalize();
		//                          가속도
		PTraceDir += Dir * _Delta * 1000.0f;

		// 최대 속력의 제한을 걸어야 합니다.

		if (700.0f <= PTraceDir.Length())
		{
			// 속력을 제한하는 연산.
			PTraceDir.Normalize();
			PTraceDir *= 700.0f;
		}

		OtherActor->AddActorWorldOffset(PTraceDir * _Delta);
		break;
	}
	case OtherState::Look:
	{
		// 각도 방향을 생각하는데.
		// 언리얼엔진에서는 
		// 방향을 넣어주면 그 방향을 바라보게 만드는 함수가 있어요
		FVector Dir = MainActor->GetActorLocation() - OtherActor->GetActorLocation();

		// FQuat 사원수로서 복소수기반 회전행렬의 약식.
		// 회전에 쓰이는 자료형중에 하나
		
		// FRotator Rot;
		// 사원수 => 로테이터가 될수 있고
		// FQuat => FRotator
		// FRotator => FQuat 될수 있는 함수를 내부게 아지고 있다.

		// 방향이 있다면 거기에서 바로 로테이션을 뺄수 있게 만들어 놨다.
		FRotator Rot = Dir.Rotation();
		// 이번에는 Set이다 이유 => 한번에 상대를 바라봐야 하니까.
		OtherActor->SetActorRotation(Rot);
		break;
	}
	case OtherState::AngleRot:
	{
		// 360를 기준으로 각도를 만들수 있다.
		FRotator Rot = FRotator::MakeFromEuler({0, 0, 360.0f * _Delta });
		// 각도로 회전시키는 기본적인 회저
		OtherActor->AddActorWorldRotation(Rot);
		// OtherActor->Look(MainActor);
		break;
	}
	case OtherState::AngleLook:
	{
		// 대부분 착각하는것이 3D공간이라고 해도 한축을 무시하고 회전하는 경우가 대부분이다.
		FVector MainPos = MainActor->GetActorLocation();
		FVector OtherPos = OtherActor->GetActorLocation();
		// 혹시라도 z축이 있을 가능성을 없애는게 보통입니다.
		MainPos.Z = 0.0f;
		OtherPos.Z = 0.0f;

		FVector Dir = MainPos - OtherPos;
		Dir.Normalize();

		FVector OtherForward = OtherActor->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		// 이동이건 회전이건 그냥 가만히 놔두면

		// HeadingAngle
		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		UE_LOG(LogTemp, Log, TEXT("%S(%u)> [%f][%f]"), __FUNCTION__, __LINE__, Angle0, Angle1);

		// FString을 쓰면 문자열을 처리할때 + 같은 연산자를 이용해서
		// 쉽게 어려운 문자열을 만들어 낼수 있다.
		FString Str = "Dir Angle : ";
		Str += FString::SanitizeFloat(Angle0);
		AddDebugText(Str);

		// 외적의 z값을 곱한게 중요한게 아니죠? 외적의 결과값이 수직이 벡터가 나오고
		// 구축이 제거되어 있어야 한다.
		if (FMath::Abs(Angle0 - Angle1) >= 1.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 100.0f * _Delta });
			OtherActor->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			OtherActor->SetActorRotation(Rot);
		}
		// AngleText = FString::SanitizeFloat(Angle, 10);

		// 같은 평면안에 존재한다는것이 중요하다. 양쪽의 z가 0이라는 한축이 일치하기 때문에 
		// XYZ각 서로 다르면 회전해야할 각도로 3개가 나와야 합니다.
		// 하나의 축만 무시해도 회전해야하는 각도는 1개로 줄게됩니다.
	}
	default:
		break;
	}
}