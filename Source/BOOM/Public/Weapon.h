// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

class UBoxComponent;
class USkeletalMeshComponent;



//Enum for type of Projectile Fire
UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType {
		EBullet			UMETA(DisplayName = "Bullet"),
		ESpread			UMETA(DisplayName = "Spread"),
		EProjectile		UMETA(DisplayName = "Projectile"),
	};
}


USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	//Ammo count
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 MaxAmmo;

	//Time to be able to fire again
	UPROPERTY(EditDefaultsOnly, Category = Config)
	float TimeBetweenShots;

	//How many shots to make in one fire
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 ShotCost;

	//Range of bullets
	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponRange;

	//Spread of bullets
	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponSpread;
};

UCLASS()
class BOOM_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	//Triggers the Fire Action
	UFUNCTION()
	void Fire();

	//Deals with Instant Types of Fire
	UFUNCTION()
	void Instant_Fire();

	//Object for accessing Struct & Enum
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	//Allows for editing Weapon Projectile Type
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	USkeletalMeshComponent* WeaponMesh;

protected:

	//Traces the weapon Fire from a point to another point
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	
	void ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir, int32 RandomSeed, float ReticleSpread);
};
