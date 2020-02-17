// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"




UENUM(BlueprintType)
namespace EAmmoTypes {
	enum AmmoTypes
	{
		ERifle			UMETA(DisplayName = "Rifle"),
		EPistol			UMETA(DisplayName = "Pistol"),
		ERocket			UMETA(DisplayName = "Rocket Launcher"),
		EShotgun		UMETA(DisplayName = "Shotgun"),
	};
}

UCLASS()
class BOOM_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	int AmmoStock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TEnumAsByte<EAmmoTypes::AmmoTypes> AmmoType;
	 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
