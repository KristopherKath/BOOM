// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardShake.h"

UForwardShake::UForwardShake() {
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.10f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = 0.25f;
	RotOscillation.Pitch.Frequency = 0.1f;
}