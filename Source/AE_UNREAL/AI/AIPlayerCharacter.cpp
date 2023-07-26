// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include <Global/GlobalGameInstance.h>
#include "Camera/CameraComponent.h"
#include <Global/ProjectTile.h>
#include "AIEnum.h"

AAIPlayerCharacter::AAIPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->SetupAttachment(RootComponent);


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->FieldOfView = 45.0f;
	CameraComponent->SetupAttachment(SpringArmComponent);
	// SpringArmComponent
}

void AAIPlayerCharacter::Tick(float _Delta)
{
	// SpringArmComponent->AddLocalRotation();
	// AddControllerYawInput(100);
}

void AAIPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		// ¿©±âÀÇ ³»¿ëÀº ¹¹³Ä?
		// DefaultPawn_MoveForward Ãß°¡µÇ´Â°Í »Ó
		// Ãà¸ÅÇÎ¸¸ ÇÏ°í ÀÖ½º´Ï´Ù.
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

	// Å°¿Í ÇÔ¼ö¸¦ ¿¬°áÇÕ´Ï´Ù.
	// ÀÌ Å°°¡ ´­¸®¸é ÀÌ ÇÔ¼ö¸¦ ½ÇÇà½ÃÄÑÁàÀÎµ¥.
	// ÃàÀÏ¶§´Â ÀÏ´Ü °è¼Ó ½ÇÇà½ÃÄÑÁà.
	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &AAIPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &AAIPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("PlayerTurn", this, &AAIPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerTurnRate", this, &AAIPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("PlayerLookUp", this, &AAIPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("PlayerLookUpRate", this, &AAIPlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("PlayerAttack", EInputEvent::IE_Pressed, this, &AAIPlayerCharacter::AttackAction);
	PlayerInputComponent->BindAction("PlayerJumpAction", EInputEvent::IE_Pressed, this, &AAIPlayerCharacter::JumpAction);
}

void AAIPlayerCharacter::BeginPlay()
{
	SetAllAnimation(MapAnimation);

	Super::BeginPlay();

	GetGlobalAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &AAIPlayerCharacter::MontageEnd);

	GetGlobalAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AAIPlayerCharacter::AnimNotifyBegin);

	SetAniState(AIAniState::Idle);
}

void AAIPlayerCharacter::AttackAction()
{
	// 여기가 아니라 몽타주가 책임져줘야 한다.

	// 이걸 여기에서 하면 안된다.


	SetAniState(AIAniState::Attack);
}
void AAIPlayerCharacter::JumpAction()
{

	Jump();
	SetAniState(AIAniState::Jump);
}
void AAIPlayerCharacter::MoveRight(float Val)
{
	AIAniState AniStateValue = GetAniState<AIAniState>();

	if (AniStateValue == AIAniState::Attack || AniStateValue == AIAniState::Jump)
	{
		return;
	}

	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();
			// transform to world space and add it
			// ÇöÀç ³» È¸ÀüÀ» °¡Á®¿Í¼­ yÃà¿¡ ÇØ´çÇÏ´Â Ãàº¤ÅÍ¸¦ ¾ò¾î¿À´Â °Ì´Ï´Ù.
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);

			SetAniState(Val > 0.f ? AIAniState::RightMove : AIAniState::LeftMove);
			return;
		}
	}
	else
	{
		if (AniStateValue == AIAniState::RightMove || AniStateValue == AIAniState::LeftMove)
		{
			SetAniState(AIAniState::Idle);
		}
	}
}
void AAIPlayerCharacter::MoveForward(float Val)
{
	AIAniState AniStateValue = GetAniState<AIAniState>();

	if (AniStateValue == AIAniState::Attack || AniStateValue == AIAniState::Jump)
	{
		return;
	}

	if (Val != 0.f)
	{
		if (Controller)
		{
			// ÄÁÆ®·Ñ·¯´Â ±âº»ÀûÀ¸·Î
			// charcter ÇÏ³ª¾¿ ºÙ¾î ÀÖ½À´Ï´Ù.
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// ÀÌ°Ç ¹æÇâÀÏ »ÓÀÔ´Ï´Ù.
			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
			// Å¾ºä°ÔÀÓÀÌ¸é ´Ù¸£°Ô ³ª¿À°Ô µÇ´Âµ¥.
			// Áö±ÝÀº TPS¸¦ ÇÏ°í ÀÖ±â ¶§¹®¿¡ ÄÁÆ®·Ñ·¯ÀÇ È¸ÀüÀÌ³ª ¾×ÅÍÀÇ È¸ÀüÀÌ³ª °°¾Æ¿ä.
			// AddMovementInput(GetActorForwardVector(), Val);

			SetAniState(Val > 0.f ? AIAniState::ForwardMove : AIAniState::BackwardMove);
			return;
		}
	}
	else
	{
		if (AniStateValue == AIAniState::ForwardMove || AniStateValue == AIAniState::BackwardMove)
		{
			SetAniState(AIAniState::Idle);
		}
	}
}
void AAIPlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void AAIPlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void AAIPlayerCharacter::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	TSubclassOf<UAnimInstance> Inst = AAIPlayerCharacter::StaticClass();

	// Anim Á¾·áµÈ ¸ùÅ¸ÁÖ
	if (MapAnimation[AIAniState::Attack] == Anim)
	{
		SetAniState(AIAniState::Idle);
	}

	if (MapAnimation[AIAniState::Jump] == Anim)
	{
		SetAniState(AIAniState::Idle);
	}
}

void AAIPlayerCharacter::AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	TSubclassOf<UObject> Effect = Inst->GetSubClass(TEXT("Effect"));

	TSubclassOf<UObject> RangeAttack = Inst->GetSubClass(TEXT("PlayerRangeAttack"));

	if (nullptr != Effect)
	{
		FTransform Trans;
		FVector Pos;
		TArray<UActorComponent*> MeshEffects = GetComponentsByTag(USceneComponent::StaticClass(), TEXT("WeaponEffect"));
		TArray<UActorComponent*> StaticMeshs = GetComponentsByTag(USceneComponent::StaticClass(), TEXT("WeaponMesh"));

		USceneComponent* EffectCom = Cast<USceneComponent>(MeshEffects[0]);
		Pos = EffectCom->GetComponentToWorld().GetLocation();

		// ÀÌÆåÆ® ¸¸µé±â
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(Effect);
			// º»ÀÇ À§Ä¡¸¦ ¾ò¾î¿À´Â ¹ý.
			// ³»°¡ ¾ò¾î¿À°í ½ÍÀº Á¤º¸°¡ ÀÖ´Ù. ´©°¡ °¡Áö°í ÀÖÀ»±î?
			//USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
			//Trans = SkeletalMeshComponent->GetSocketTransform(TEXT("B_R_Weapon"));
			//Pos = Trans.GetLocation();
			// UActorComponent* ActorComponent = GetComponentByClass(UStaticMeshComponent::StaticClass());
			// TArray<UActorComponent*> ActorComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
			// UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponents[0]);
			// Pos = StaticMeshComponent->GetComponentToWorld().GetLocation();
			Actor->SetActorLocation(Pos);
			Actor->SetActorRotation(GetActorRotation());
			Actor->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
		}

		// ¹ß»çÃ¼ ¸¸µé±â
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(RangeAttack);

			AProjectTile* ProjectTile = Cast<AProjectTile>(Actor);

			ProjectTile->SetActorLocation(Pos);
			ProjectTile->SetActorRotation(GetActorRotation());
			ProjectTile->GetSphereComponent()->SetCollisionProfileName(TEXT("PlayerAttack"), true);
		}
	}
}