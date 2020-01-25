// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "MainMenuHUD.h"
#include "UObject/ConstructorHelpers.h"

AMainMenuGameMode::AMainMenuGameMode() : Super() {
	HUDClass = AMainMenuHUD::StaticClass();

	ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/Blueprints/PlayerController/BP_Main_Menu_Controller"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;
}