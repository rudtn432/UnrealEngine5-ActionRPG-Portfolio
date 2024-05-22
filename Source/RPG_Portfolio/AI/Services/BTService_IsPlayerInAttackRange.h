// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class RPG_PORTFOLIO_API UBTService_IsPlayerInAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_IsPlayerInAttackRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	float Attack_Range = 25.0f;

};
