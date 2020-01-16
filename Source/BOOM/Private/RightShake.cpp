// Fill out your copyright notice in the Description page of Project Settings.


#include "RightShake.h"

URightShake::URightShake(){
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.15f;
	OscillationBlendOutTime = 0.15f;

	RotOscillation.Yaw.Amplitude = FMath::RandRange(-3.f, 3.f);
	RotOscillation.Yaw.Frequency = 1.f;
}