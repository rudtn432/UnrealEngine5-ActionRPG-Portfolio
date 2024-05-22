// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "blackboard_keys.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = Cast<AMyAIController>(owner_comp.GetAIOwner());
	FVector const player_location = cont->Get_Blackboard()->GetValueAsVector(bb_keys::random_loc_vector);
	if (!cont->Get_Blackboard()->GetValueAsBool(bb_keys::be_hit))
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, player_location);
	
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
