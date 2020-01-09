// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class BOOM_API UChooseNextWaypoint : public UBTTaskNode
{
	GENERATED_BODY()
		//Inheritance (i think) of BTTaskNode for the sake of the C++ class
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	//Creating variables for BlackBoard Keys
protected:
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
		struct FBlackboardKeySelector Index;

};
