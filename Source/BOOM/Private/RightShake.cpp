// Fill out your copyright notice in the Description page of Project Settings.


#include "RightShake.h"

URightShake::URightShake(){
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.10f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Roll.Amplitude = 0.25f;
	RotOscillation.Roll.Frequency = 0.1f;
}