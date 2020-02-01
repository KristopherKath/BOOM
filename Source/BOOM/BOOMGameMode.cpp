// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BOOMGameMode.h"
#include "BOOMCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Containers/Array.h"
#include "LevelEnter.h"


ABOOMGameMode::ABOOMGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;


	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/Blueprints/PlayerController/BP_BOOM_Controller"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;


}

