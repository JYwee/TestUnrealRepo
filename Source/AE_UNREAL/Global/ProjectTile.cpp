// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/ProjectTile.h"

// Sets default values
AProjectTile::AProjectTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//SphereComponent->SetupAttachment(RootComponent);
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"), true);
}

// Called when the game starts or when spawned
void AProjectTile::BeginPlay()
{
	Super::BeginPlay();

	OnDestroyed.AddDynamic(this, &AProjectTile::DestroyProjectile);
	// SphereComponent->SetCollisionProfileName(TEXT("MonsterAttack"), true);
}

void AProjectTile::DestroyProjectile(AActor* _Destroy)
{
	if (nullptr == DeathCreateObject)
	{
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor<AActor>(DeathCreateObject);

	Actor->SetActorLocation(GetActorLocation());
	Actor->SetActorRotation(GetActorRotation());
}

// DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(AActor*, DestroyedActor);

// Called every frame
void AProjectTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeathTime -= DeltaTime;

	if (DeathTime < 0.0f)
	{
		Destroy();
		return;
	}

	AddActorWorldOffset(GetActorForwardVector() * DeltaTime * Speed);

}

