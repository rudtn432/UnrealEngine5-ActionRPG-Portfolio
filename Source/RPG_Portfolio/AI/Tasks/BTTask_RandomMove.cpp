// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandomMove.h"
#include "RPG_Portfolio.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "NavigationSystem.h"
#include "MyAIController.h"
#include "Engine.h"
#include "Engine/World.h"
#include "blackboard_keys.h"


UBTTask_RandomMove::UBTTask_RandomMove(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* nodeMemory)
{
	auto const cont = Cast<AMyAIController>(owner_comp.GetAIOwner());
	auto const npc = cont->GetPawn();

	FVector const origin = npc->GetActorLocation();
	FNavLocation loc;

	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr)
		&& !cont->Get_Blackboard()->GetValueAsBool(bb_keys::be_hit))
	{
		cont->Get_Blackboard()->SetValueAsVector(bb_keys::random_loc_vector, loc.Location);
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
