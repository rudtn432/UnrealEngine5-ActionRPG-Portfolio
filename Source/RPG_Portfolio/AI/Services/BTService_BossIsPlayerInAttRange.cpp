// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossIsPlayerInAttRange.h"
#include "MyBossAIController.h"
#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "blackboard_keys.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_BossIsPlayerInAttRange::UBTService_BossIsPlayerInAttRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Boss Is Player in Attack Range");
}

void UBTService_BossIsPlayerInAttRange::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);

	AMyBossAIController* const cont = Cast<AMyBossAIController>(owner_comp.GetAIOwner());
	ABossCharacter* const boss = Cast<ABossCharacter>(cont->GetPawn());

	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		cont->Get_Blackboard()->SetValueAsBool(bb_keys::player_is_in_BossAttackRange, boss->GetDistanceTo(player) <= Attack_Range);
		if (cont->Get_Blackboard()->GetValueAsBool(bb_keys::player_is_in_BossAttackRange))
		{
			cont->Get_Blackboard()->SetValueAsBool(bb_keys::chase_player, false);
			cont->Get_Blackboard()->SetValueAsBool(bb_keys::is_Wandering, true);
			boss->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		}
		else
		{
			cont->Get_Blackboard()->SetValueAsBool(bb_keys::chase_player, true);
			cont->Get_Blackboard()->SetValueAsBool(bb_keys::is_Wandering, false);
			boss->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		}
	}
}
