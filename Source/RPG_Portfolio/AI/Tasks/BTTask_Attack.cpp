// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MyAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "MyCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "blackboard_keys.h"
#include "CombatInterface.h"

UBTTask_Attack::UBTTask_Attack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = Cast<AMyAIController>(owner_comp.GetAIOwner());
	AEnemyCharacter* const npcBot = Cast<AEnemyCharacter>(cont->GetPawn());

	if (ICombatInterface* const iCombat = Cast<ICombatInterface>(npcBot))
	{
		if (montage_has_finished(npcBot) 
			&& cont->Get_Blackboard()->GetValueAsBool(bb_keys::player_is_in_AttackRange) 
			&& !cont->Get_Blackboard()->GetValueAsBool(bb_keys::be_hit))
		{
			iCombat->Execute_melee_attack(npcBot);
		}
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_Attack::montage_has_finished(AEnemyCharacter* const npcBot)
{
	return npcBot->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npcBot->GetCurrentMontage());
}
