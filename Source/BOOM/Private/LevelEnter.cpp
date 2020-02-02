// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEnter.h"
#include "Components/BoxComponent.h"
#include "BOOMCharacter.h"
#include "BOOMGameMode.h"
#include "Engine.h"


// Sets default values
ALevelEnter::ALevelEnter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	RootComponent = BoxCollider;
	
}

// Called when the game starts or when spawned
void ALevelEnter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelEnter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelEnter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ABOOMCharacter* MyCharacter = Cast<ABOOMCharacter>(OtherActor);
	if (MyCharacter) {
		//start Level
	}
}




