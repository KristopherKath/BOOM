// Fill out your copyright notice in the Description page of Project Settings.
/*

#include "Shotgun.h"
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
#include "Sound/SoundCue.h"
#include "Math/Rotator.h"
#include "Engine.h"

//Adds Console Command for Weapon Drawing Debuging
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);


void AShotgun::Fire()
{


	// Trace the world from pawn eyes to crosshair location (center screen)
	if (MyPawn && CurrentAmmo > 0)
	{

		//In shoot code
		FVector AimDir = GetAdjustedAim();
		FVector StartTrace = GetCameraDamageStartLocation(AimDir);

		int RandomX = FMath::RandRange(-20, 20);
		int RandomY = FMath::RandRange(-20, 20);
		int RandomZ = FMath::RandRange(-20, 20);

		FRotator RandomAimRot = AimDir.Rotator();
		RandomAimRot += FRotator(RandomX, RandomY, RandomZ) * SpreadAmount;

		FVector EndTrace = (StartTrace)+(RandomAimRot.Vector() * WeaponRange);



		SpreadAmount += 0.3;
		if (SpreadAmount > 1) SpreadAmount = 1;




		// Retrieves the eye location and rotation of actor
		FVector EyeLocation;
		FRotator EyeRotation;
		MyPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		//Get Direction
		FVector ShotDirection = EyeRotation.Vector();

		FRotator RandomAimRot = ShotDirection.Rotator();

		// Get the vector end of a line trace
		FVector TraceEnd = EyeLocation + (ShotDirection * WeaponRange);

		// Particle "Target" parameter. It Changes if we hit something
		FVector TracerEndPoint = TraceEnd;







		// Used for more accurate collision detection.
		//Useful for getting specific hit sections for things like a headshot 
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

void AShotgun::Tick(float DeltaTime)
{
	Super::Tick();

	if (SpreadAmount > 0)
	{
		SpreadAmount -= DeltaTime * 1;

		if (SpreadAmount < 0) SpreadAmount = 0;
	}
}
*/