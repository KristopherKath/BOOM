// Fill out your copyright notice in the Description page of Project Settings.

//#include"BOOM.h"
#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrollingGuard.h" //TODO remove couplng
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Patrol Points from enemy instance 
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	//Get Points
	auto PatrolPoints = PatrollingGuard->PatrolPointsCPP;

	//Set next waypoint
	//Retrieve BlackBoard data for AI
	auto BlackBordComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackBordComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackBordComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	//Cylce Index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	//Index = NextIndex;
	BlackBordComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	UE_LOG(LogTemp, Warning, TEXT("Waypoint index : %i"),Index);

	//Node equivalent of task succeeded
	//must have return statement in every C++ class
	 return EBTNodeResult::Succeeded;
}
