// Fill out your copyright notice in the Description page of Project Settings.

#include"BOOM.h"
#include "ChooseNextWaypoint.h"
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Comment to show Class is wokring in Engine
	UE_LOG(LogTemp, Warning, TEXT("AI in C++ Test"));
	//Node equivalent of task succeeded
	 return EBTNodeResult::Succeeded;
}
