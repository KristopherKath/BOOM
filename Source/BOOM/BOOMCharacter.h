// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BOOMCharacter.generated.h"

class UInputComponent;
class AWeapon;

UCLASS(config=Game)
class ABOOMCharacter : public ACharacter
{
	GENERATED_BODY()

	
	

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;


	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* L_MotionController;



	UPROPERTY(VisibleAnywhere, Category = "Player")
	TSubclassOf<AWeapon> StarterWeaponClass;


	AWeapon* CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	void StartFire();

	void StopFire();



	UFUNCTION()
	virtual void BeginPlay() override;

public:
	ABOOMCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

public:

	/** The Maximum number of Jump for the Character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		int MaxJump = 2;

	/** The Jump Force of the Character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		float JumpForce = 1500.f;

	/** The Forward Jump Force of the Character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		float ForwardJumpForce = 500.f;

	/** The Maximum number of Jump for the Character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		int MaxDash = 1;

	/** Delay Time between dashes*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		float DashDelay = 3.f;

	/** The forward force of the dash*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		float DashForce = 2000.f;
	
	/** The ratio of force after the dash the dash*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		float BackwardRatio = 0.5f;

	/** The additional force of the player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		float FallingForce = -15.f;

	/** Double Jump Handling */
	void DoubleJump();
	void Dash();
	virtual void Landed(const FHitResult& Hit) override;
	virtual bool IsFalling();

	/** Camera Swaying Handling */
	/** Right Shake */
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> rightShake;
	/** Forward Shake */
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> forwardShake;

protected:
	UPROPERTY(BlueprintReadOnly)
		float AbilityCooldown = 1.f;

private:
	int JumpCounter = 0;
	int DashCounter = 0;
	
	FVector LaunchVec;
	void SlowDownDash();
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ABOOMProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

protected:



	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface



public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

