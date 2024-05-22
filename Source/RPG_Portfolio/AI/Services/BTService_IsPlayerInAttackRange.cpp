// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInAttackRange.h"
#include "MyAIController.h"
#include "EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UBTService_IsPlayerInAttackRange::UBTService_IsPlayerInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Attack Range");
}

void UBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);

	AMyAIController* const cont = Cast<AMyAIController>(owner_comp.GetAIOwner());
	AEnemyCharacter* const npcBot = Cast<AEnemyCharacter>(cont->GetPawn());

	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		cont->Get_Blackboard()->SetValueAsBool(bb_keys::player_is_in_AttackRange, npcBot->GetDistanceTo(player) <= Attack_Range);
	}
}

