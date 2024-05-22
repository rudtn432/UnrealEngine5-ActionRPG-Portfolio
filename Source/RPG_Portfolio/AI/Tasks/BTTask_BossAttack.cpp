// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossAttack.h"
#include "blackboard_keys.h"
#include "MyBossAIController.h"
#include "BossCharacter.h"
#include "CombatInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BossAttack::UBTTask_BossAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Boss Attack");
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = Cast<AMyBossAIController>(owner_comp.GetAIOwner());
	ABossCharacter* const boss = Cast<ABossCharacter>(cont->GetPawn());

	if (ICombatInterface* const iCombat = Cast<ICombatInterface>(boss))
	{
		if (montage_has_finished(boss)
			&& cont->Get_Blackboard()->GetValueAsBool(bb_keys::player_is_in_BossAttackRange)
			&& !cont->Get_Blackboard()->GetValueAsBool(bb_keys::is_Wandering))
		{
			iCombat->Execute_melee_attack(boss);
		}
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_BossAttack::montage_has_finished(ABossCharacter* const boss)
{
	return boss->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(boss->GetCurrentMontage());
}
