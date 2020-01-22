// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingShake.h"

UShootingShake::UShootingShake() {
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.10f;
	OscillationBlendOutTime = 0.10f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(1.0f, 2.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(2.0f, 4.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(1.0f, 2.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(2.0f, 4.0f);
}