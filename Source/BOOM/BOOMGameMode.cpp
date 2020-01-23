// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BOOMGameMode.h"
#include "BOOMHUD.h"
#include "BOOMCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABOOMGameMode::ABOOMGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABOOMHUD::StaticClass();
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/GameMode/PlayerController/BP_BOOM_Controller"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;
}
