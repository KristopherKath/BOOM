// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardShake.h"

UForwardShake::UForwardShake() {
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.10f;
	OscillationBlendOutTime = 0.10f;

	RotOscillation.Roll.Amplitude = FMath::RandRange(-3.f, 3.f);
	RotOscillation.Roll.Frequency = 0.1f;
}