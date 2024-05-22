// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossChasePlayer.h"
#include "MyBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "blackboard_keys.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BossChasePlayer::UBTTask_BossChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Boss Chase Player");
}

EBTNodeResult::Type UBTTask_BossChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		auto const cont = Cast<AMyBossAIController>(owner_comp.GetAIOwner());
		FVector const player_location = player->GetActorLocation();

		if (cont->Get_Blackboard()->GetValueAsBool(bb_keys::chase_player))
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, player_location);
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
