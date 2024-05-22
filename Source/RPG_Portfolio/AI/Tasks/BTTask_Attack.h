// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyCharacter.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class RPG_PORTFOLIO_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;


private:
	bool montage_has_finished(AEnemyCharacter* const npcBot);
};
