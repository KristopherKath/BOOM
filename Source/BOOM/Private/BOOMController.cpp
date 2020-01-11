// Fill out your copyright notice in the Description page of Project Settings.


#include "BOOMController.h"
#include "Blueprint/UserWidget.h"

void ABOOMController::BeginPlay() {
	Super::BeginPlay();
	if (wPlayerUI)  // Check if the Asset is assigned in the blueprint.
	{
		PlayerUI = CreateWidget<UUserWidget>(this, wPlayerUI);

		// now you can use the widget directly since you have a reference for it.
		// Extra check to  make sure the pointer holds the widget.
		if (PlayerUI)
		{
			//let add it to the view port
			PlayerUI->AddToViewport();
		}

	}
}

