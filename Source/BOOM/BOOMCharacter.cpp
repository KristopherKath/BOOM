// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BOOMCharacter.h"
#include "BOOMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "Engine/Blueprint.h"
#include "Weapon.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "GameFramework/Actor.h"
#include "RocketLauncher.h"
#include "AmmoPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ABOOMCharacter

ABOOMCharacter::ABOOMCharacter()
{

	CurrentWeapon = NULL;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	CollisionComp = GetCapsuleComponent();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABOOMCharacter::OnCollision);

	//Sets Inventory size
	Inventory.SetNum(4, false);


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

	bStartWithDefaultWeapon = true;
}


void ABOOMCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (bStartWithDefaultWeapon)
	{
		GiveDefaultWeapon();
	}
}

//Detects Collision
void ABOOMCharacter::OnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If Cast successful then pick up weapon 
	AWeapon* Weapon = Cast<AWeapon>(OtherActor);
	if (Weapon)
	{
		ProcessWeaponPickup(Weapon);
	}
	AAmmoPickup* Ammo = Cast<AAmmoPickup>(OtherActor);
	if (Ammo)
	{
		ProcessAmmoPickup(Ammo);
	}
}

//Picks up weapon and puts it into inventory, else take the ammo
void ABOOMCharacter::ProcessWeaponPickup(AWeapon* Weapon)
{
	//If weapon exists
	if (Weapon != NULL)
	{
		//Turns off collision on weapon picked up (?)
		UBoxComponent* CollisionComp = Cast<UBoxComponent>(Weapon->GetComponentByClass(UBoxComponent::StaticClass()));
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//If the weapon is not already picked up
		if (Inventory[Weapon->Priority] == NULL)
		{
			//Add weapon to inventory
			AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner)
			{
				Inventory[Spawner->Priority] = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Picked up " + Inventory[Spawner->Priority]->Name);
			}
			Weapon->Destroy();

			//If there is no current weapon equipped then equip this weapon
			if (CurrentWeapon == NULL)
			{
				EquipWeapon(Inventory[Spawner->Priority]);
			}
		}
		else
		{
			if (Inventory[Weapon->Priority]->CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (Inventory[Weapon->Priority]->AmmoCapacity - Inventory[Weapon->Priority]->CurrentAmmo))
			{
				Inventory[Weapon->Priority]->CurrentAmmo += Weapon->CurrentAmmo;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Added " + Weapon->CurrentAmmo);
				Weapon->Destroy();
			}
			if (Inventory[Weapon->Priority]->CurrentAmmo > Inventory[Weapon->Priority]->AmmoCapacity)
			{
				Inventory[Weapon->Priority]->CurrentAmmo = Inventory[Weapon->Priority]->AmmoCapacity;
			}
		}
	}
}

//Make next weapon the current weapon
void ABOOMCharacter::NextWeapon()
{
	if (Inventory[CurrentWeapon->Priority]->Priority != Inventory.Num() - 1)
	{
		if (Inventory[CurrentWeapon->Priority + 1] == NULL)
		{
			for (int i = CurrentWeapon->Priority + 1; i < Inventory.Num(); ++i)
			{
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(Inventory[i]);
				}
			}
		}
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->Priority + 1]);
		}
	}
	else
	{
		EquipWeapon(Inventory[CurrentWeapon->Priority]);
	}
}

//Make previous weapon the current weapon
void ABOOMCharacter::PrevWeapon()
{
	if (Inventory[CurrentWeapon->Priority]->Priority != 0)
	{
		if (Inventory[CurrentWeapon->Priority - 1] == NULL)
		{
			for (int i = CurrentWeapon->Priority - 1; i >= 0; --i)
			{
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(Inventory[i]);
				}
			}
		}
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->Priority - 1]);
		}
	}
	else
	{
		EquipWeapon(Inventory[CurrentWeapon->Priority]);
	}
}

//Make given weapon the weapon for player to use
void ABOOMCharacter::EquipWeapon(AWeapon* Weapon)
{
	if (CurrentWeapon != NULL)
	{
		//Turn off weapon collision after picking up
		UBoxComponent* CollisionComp = Cast<UBoxComponent>(Weapon->GetComponentByClass(UBoxComponent::StaticClass()));
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Unequip our weapon
		CurrentWeapon = Inventory[CurrentWeapon->Priority];
		CurrentWeapon->OnUnEquip();

		//Set new current weapon and equip
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
	else 
	{
		UBoxComponent* CollisionComp = Cast<UBoxComponent>(Weapon->GetComponentByClass(UBoxComponent::StaticClass()));
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Equip given weapon
		CurrentWeapon = Weapon;
		CurrentWeapon = Inventory[CurrentWeapon->Priority];
		CurrentWeapon->SetOwningPawn(this);
		Weapon->OnEquip(); 
	}
}

//Give player a default weapon on Start
void ABOOMCharacter::GiveDefaultWeapon()
{
	//Spawns and equips a default weapon
	AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(StarterWeaponClass);
	if (Spawner)
	{
		Inventory[Spawner->Priority] = Spawner;
		CurrentWeapon = Inventory[Spawner->Priority];
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->OnEquip();
	}
}

//Looks for ammo type in inventory and then adds it weapons ammo
void ABOOMCharacter::ProcessAmmoPickup(AAmmoPickup* Ammo)
{
	//If a weapon exists then add the ammo
	if (CurrentWeapon)
	{
		if (Ammo != NULL)
		{
			//Add ammo for equipped weapon
			if (CurrentWeapon->AmmoType == Ammo->AmmoType)
			{
				CurrentWeapon->AddAmmo(Ammo->AmmoStock);
				Ammo->Destroy();
			}
			//Add ammo for weapon in inventory
			else {
				for (int i = 0; i < Inventory.Num(); ++i)
				{
					if (Inventory[i] != NULL) {
						if (Inventory[i]->AmmoType == Ammo->AmmoType)
						{
							Inventory[i]->AddAmmo(Ammo->AmmoStock);
							Ammo->Destroy();
						}
					}
				}
			}
		}
	}
}



void ABOOMCharacter::StartFire()
{
	if (CurrentWeapon != NULL && !inputDisabled)
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
	if (AbilityCooldown < 1 && !inputDisabled) {
		AbilityCooldown += (DeltaTime/DashDelay);
	}
	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ABOOMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	//Bind Invenotry events
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ABOOMCharacter::NextWeapon);
	PlayerInputComponent->BindAction("PrevWeapon", IE_Pressed, this, &ABOOMCharacter::PrevWeapon);

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
	if (JumpCounter < MaxJump && !inputDisabled) {
		JumpCounter++;
		LaunchVec = GetLastMovementInputVector();
		LaunchCharacter(FVector(LaunchVec.X*ForwardJumpForce, LaunchVec.Y*ForwardJumpForce, JumpForce), true, true);
	}
}

void ABOOMCharacter::Dash() {
	if (AbilityCooldown >= 1 && !inputDisabled) {
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
	if (Value != 0.0f && !inputDisabled)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(forwardShake);
	}
}

void ABOOMCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !inputDisabled)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(rightShake);
	}
}

void ABOOMCharacter::TurnAtRate(float Rate)
{
	if (!inputDisabled)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	}
}

void ABOOMCharacter::LookUpAtRate(float Rate)
{
	if (!inputDisabled) {
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
	
}

