// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BOOMCharacter.h"
#include "BOOMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget.h"

#include "Engine.h"
#include "Engine/Blueprint.h"
#include "Weapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ABOOMCharacter

ABOOMCharacter::ABOOMCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	WeaponAttachSocketName = "Weapon_Socket";

}


void ABOOMCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn a default weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}
}


void ABOOMCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}


void ABOOMCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}


void ABOOMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsFalling()) {
		LaunchCharacter(FVector(0,0, FallingForce), false, false);
		if (JumpCounter == 0) {
			JumpCounter++;
		}
	}
	if (AbilityCooldown < 1) {
		AbilityCooldown += (DeltaTime/DashDelay);
	}
	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ABOOMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABOOMCharacter::Dash);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABOOMCharacter::DoubleJump);

	//Fire Input
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABOOMCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABOOMCharacter::StopFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ABOOMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABOOMCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABOOMCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABOOMCharacter::LookUpAtRate);
}


void ABOOMCharacter::DoubleJump() {
	if (JumpCounter < MaxJump) {
		JumpCounter++;
		LaunchVec = GetLastMovementInputVector();
		LaunchCharacter(FVector(LaunchVec.X*ForwardJumpForce, LaunchVec.Y*ForwardJumpForce, JumpForce), true, true);
	}
}

void ABOOMCharacter::Dash() {
	if (AbilityCooldown >= 1) {
		LaunchVec = GetLastMovementInputVector();
		if (DashCounter < MaxDash && !(LaunchVec.IsNearlyZero())) {
			DashCounter++;
			LaunchCharacter(FVector(LaunchVec.X*DashForce, LaunchVec.Y*DashForce, JumpForce / 2), true, true);
			AbilityCooldown = 0;
		}

	}
}

void ABOOMCharacter::SlowDownDash() {
	LaunchCharacter(FVector(LaunchVec.X*DashForce*BackwardRatio, LaunchVec.Y*DashForce*BackwardRatio, 0), true, false);
}
void ABOOMCharacter::Landed(const FHitResult& Hit) {
	JumpCounter = 0;
	DashCounter = 0;

}

bool ABOOMCharacter::IsFalling(){
	if (GetVelocity().Z < 0) {
		return true;
	}
	else
	{
		return false;
	}
	
}

void ABOOMCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(forwardShake);
	}
}

void ABOOMCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(rightShake);
	}
}

void ABOOMCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABOOMCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

