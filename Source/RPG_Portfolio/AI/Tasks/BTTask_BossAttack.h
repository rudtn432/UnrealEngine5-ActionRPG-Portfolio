// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BossCharacter.h"
#include "BTTask_BossAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPG_PORTFOLIO_API UBTTask_BossAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_BossAttack(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;


private:
	bool montage_has_finished(ABossCharacter* const boss);
};
