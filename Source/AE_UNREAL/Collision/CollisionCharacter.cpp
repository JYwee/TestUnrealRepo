// Fill out your copyright notice in the Description page of Project Settings.


#include "Collision/CollisionCharacter.h"
#include "Components/CapsuleComponent.h"
#include <Global/ARGlobal.h>

// Sets default values
ACollisionCharacter::ACollisionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollisionCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 충돌용 이벤트는 각충돌 컴포넌트마다 지정해줘야 합니다.

	// UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&ACollisionCharacter::BeginOverLap);
	
}

// Called every frame
void ACollisionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 언리얼에서 Trace충돌은 월드를 통해서 직접해야한다.

	// 아래의 함수들은 지금 이순간 충돌체를 만드는 방식이다.

	// GetWorld()->LineTraceSingleByProfile

	// 난 이함수를 왜 쓰려고 하는가?
	
	// 이 함수를 지금 어떤 함수로서 파악하고 있는가?

	TArray<FHitResult> OutHits; // 결과를 받는 자료구조
	FVector Start = GetActorLocation() - FVector(0.0f, 0.0f,GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); // 선의 시작
	FVector End = GetActorLocation() + (-GetActorUpVector() * 1000000.0f); // 선의 끝
	FName ProfileName = TEXT("GroundCheck"); // 나랑 충돌할 충돌 프리셋
	                                     // 충돌을 체크해서 다른 오브젝트들을 파악할 프리셋

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;

	GetWorld()->LineTraceMultiByProfile(OutHits, Start, End, ProfileName, Params);

	if (0 != OutHits.Num())
	{
		{
			FString Text;
			Text = OutHits[0].GetActor()->GetName();
			UARGlobal::ARDebugTextPrint(this, Text);
		}

		{
			FString Text = "Dis : ";
			Text += FString::SanitizeFloat(OutHits[0].Distance);
			UARGlobal::ARDebugTextPrint(this, Text);
		}
	}

}

// Called to bind functionality to input
void ACollisionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		// 여기의 내용은 뭐냐?
		// DefaultPawn_MoveForward 추가되는것 뿐
		// 축매핑만 하고 있스니다.
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::S, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::D, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerTurn", EKeys::MouseX, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerJumpAxis", EKeys::E, -1.f));


		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerAttack"), EKeys::LeftMouseButton));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerJumpAction"), EKeys::SpaceBar));
	}

	// 키와 함수를 연결합니다.
	// 이 키가 눌리면 이 함수를 실행시켜줘인데.
	// 축일때는 일단 계속 실행시켜줘.
	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &ACollisionCharacter::MoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &ACollisionCharacter::MoveRight);
	PlayerInputComponent->BindAxis("PlayerTurn", this, &ACollisionCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerTurnRate", this, &ACollisionCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("PlayerLookUp", this, &ACollisionCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("PlayerLookUpRate", this, &ACollisionCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("PlayerJumpAction", EInputEvent::IE_Pressed, this, &ACollisionCharacter::JumpAction);
}



void ACollisionCharacter::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);
			return;
		}
	}
}

void ACollisionCharacter::MoveForward(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
			return;
		}
	}
}


void ACollisionCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void ACollisionCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}


void ACollisionCharacter::JumpAction()
{
	Jump();
}



void ACollisionCharacter::BeginOverLap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// 나와 겹친 오브젝트가 파괴되기를 원한다.

	if (OtherActor->ActorHasTag(TEXT("ItEM")))
	{
		OtherActor->Destroy();
	}

	// OverlappedComponent 충돌한 이 엑터가 가지고 있는 컴포넌트 
	// OtherActor 충돌한 상대 actor => Item
	// OtherComp 충돌한 상대 컴포넌트

	// int32 OtherBodyIndex, 지금 컨텐츠적으로 별로 중요하지 않기 때문에 pass
	// bool bFromSweep, 지금 컨텐츠적으로 별로 중요하지 않기 때문에 pass


	// FHitResult 은 충돌에 대한 세부 정보를 가지고 있다.
	SweepResult.Distance;
	// 충돌체크를 통해서 파악된 상대와의 거리
	SweepResult.FaceIndex;
	// 충돌한 면의 번호
	SweepResult.GetActor();
	// 충돌한 액터

}
