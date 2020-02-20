// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Engine.h"
#include "..\Public\Weapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials\PhysicalMaterial.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "BOOM.h"
#include "BOOMCharacter.h"


//Adds Console Command for Weapon Drawing Debuging
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);


// Sets default values
AWeapon::AWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	BaseDamage = 20.0f;

	//Bullets per minute
	RateOfFire = 600;

	WeaponRange = 1500;

	AmmoCapacity = 30;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = AmmoCapacity;
	TimeBetweenShots = 60 / RateOfFire;
}

void AWeapon::Fire()
{
	// Trace the world from pawn eyes to crosshair location (center screen)
	if (MyPawn && CurrentAmmo > 0)
	{
		// Retrieves the eye location and rotation of actor
		FVector EyeLocation;
		FRotator EyeRotation;
		MyPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		//Get Direction
		FVector ShotDirection = EyeRotation.Vector();

		// Get the vector end of a line trace
		FVector TraceEnd = EyeLocation + (ShotDirection * WeaponRange);

		// Particle "Target" parameter. It Changes if we hit something
		FVector TracerEndPoint = TraceEnd;


		/* Used for more accurate collision detection.
		Useful for getting specific hit sections for things like a headshot */
		FCollisionQueryParams QueryParams;
		//Ignore Character & Weapon
		QueryParams.AddIgnoredActor(MyPawn);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		// A struct of hit result data
		FHitResult Hit;
		// if there was a hit
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, TRACE_WEAPON, QueryParams))
		{
			// Blocking hit! Process damage
			AActor* HitActor = Hit.GetActor();

			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDamage = BaseDamage;
			//If critical hit change actualDamage var
			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 4.0f;
			}

			//Applies damage to hit actor
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyPawn->GetInstigatorController(), this, DamageType);


			//Determine Surface Type
			UParticleSystem* SelectedEffect = nullptr;

			//Select what impact effect to play.
				//Case names defined in BOOM.h
			switch (SurfaceType)
			{
			case SURFACE_FLESHDEFAULT:
			case SURFACE_FLESHVULNERABLE:
				SelectedEffect = FleshImpactEffect;
				break;
			default:
				SelectedEffect = DefaultImpactEffect;
				break;
			}

			// Play ImpactEffect
			if (SelectedEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
			//Change end point if we hit something
			TracerEndPoint = Hit.ImpactPoint;
		}

		if (DebugWeaponDrawing > 0)
		{
			// Draw A line for the shot
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
		}

		PlayFireEffects(TracerEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;

		CurrentAmmo--;
	}

}

//Makes this weapon's owner the pawn passed in
void AWeapon::SetOwningPawn(ABOOMCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

//Attaches the weapon to the character
void AWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent* Character = MyPawn->GetMesh1P();
		MeshComp->SetHiddenInGame(false);
		MeshComp->AttachTo(Character, "Weapon_Socket");
	}
}

//Removes the weapon from character and hides it
void AWeapon::DetachFromPlayer()
{
	MeshComp->DetachFromParent();
	MeshComp->SetHiddenInGame(true);
}

void AWeapon::OnEquip()
{
	AttachToPlayer();
}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

void AWeapon::StartFire()
{
	//Pick which ever value is greatest. Left or 0. Reason being is that negative value is default in SetTimer()
	//And if we use that then it would disregaurd the FirstDelay function we want.
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	//Every n second we call Fire()
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void AWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}



// Plays Effects
void AWeapon::PlayFireEffects(FVector TracerEndPoint)
{
	// Play MuzzleEffect
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}


	// Play particle trace effect
	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}

	//Used for Shaking the player camera
	if (MyPawn)
	{
		APlayerController* PC = Cast<APlayerController>(MyPawn->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}

}

void AWeapon::AddAmmo(int addAmount) 
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + addAmount, 0, AmmoCapacity);
}