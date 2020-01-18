// Fill out your copyright notice in the Description page of Project Settings.

#include"BOOM.h"
#include "ChooseNextWaypoint.h"
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Retrieve BlackBoard data for AI
	auto BlackBordComp = OwnerComp.GetBlackboardComponent();
	//Comment to show code is wokring in Engine
	UE_LOG(LogTemp, Warning, TEXT("AI in C++ Test"));
	//Node equivalent of task succeeded
	//must have return statement in every C++ class
	 return EBTNodeResult::Succeeded;
}
