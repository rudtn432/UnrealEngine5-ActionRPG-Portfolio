// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "MyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "blackboard_keys.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		auto const cont = Cast<AMyAIController>(owner_comp.GetAIOwner());
		FVector const player_location = player->GetActorLocation();

		if (search_random)
		{
			FNavLocation loc;

			UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
			if (nav_sys->GetRandomPointInNavigableRadius(player_location, search_radius, loc, nullptr)
				&& !cont->Get_Blackboard()->GetValueAsBool(bb_keys::be_hit))
			{
				cont->Get_Blackboard()->SetValueAsVector(bb_keys::random_loc_vector, loc.Location);
			}
		}
		else
		{
			if (!cont->Get_Blackboard()->GetValueAsBool(bb_keys::be_hit))
				cont->Get_Blackboard()->SetValueAsVector(bb_keys::random_loc_vector, player_location);
		}
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
