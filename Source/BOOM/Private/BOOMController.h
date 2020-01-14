// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BOOMController.generated.h"

/**
 * 
 */
UCLASS()
class ABOOMController : public APlayerController
{
	GENERATED_BODY()
	// Reference UMG Asset in the Editor
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wPlayerUI;

	// Variable to hold the widget After Creating it.
	UUserWidget* PlayerUI;

	// Override BeginPlay()
	virtual void BeginPlay() override;
};
