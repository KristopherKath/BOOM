// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


class ABOOMCharacter;
class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class USoundCue;
class UCameraShake;

UENUM(BlueprintType)
namespace EWeaponAmmoTypes {
	enum WeaponAmmoType
	{
		ERifle			UMETA(DisplayName = "Rifle"),
		EPistol			UMETA(DisplayName = "Pistol"),
		ERocket			UMETA(DisplayName = "Rocket Launcher"),
		EShotgun		UMETA(DisplayName = "Shotgun"),
	};
}

UCLASS()
class BOOM_API AWeapon : public AActor
{
	GENERATED_BODY()



public:
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TEnumAsByte<EWeaponAmmoTypes::WeaponAmmoType> AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int Priority;


	void OnEquip();
	void OnUnEquip();

	void SetOwningPawn(ABOOMCharacter* NewOwner);

protected:

	virtual void BeginPlay() override;
	
	int BulletsPerShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	void PlayFireEffects(FVector);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;
	
	//Sound effect
	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue* FireSound;

	//Plays sound effect
	virtual void PlayWeaponSound(USoundCue* Sound);


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	virtual void Fire();

	FTimerHandle TimerHandle_TimeBetweenShots;

	float TimeBetweenShots;

	float LastFireTime;

	/* RPM - Bullets per minute fired by weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	// Range for shots
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float WeaponRange;

	void AttachToPlayer();
	void DetachFromPlayer();

	ABOOMCharacter* MyPawn;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int AmmoCapacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentAmmo;
	
	/** Boolean value for the weapon if it is firing*/
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		bool isFiring = false;
	
	
	void StartFire();

	void StopFire();

	UFUNCTION(BlueprintCallable)
	void AddAmmo(int addAmount);
};

