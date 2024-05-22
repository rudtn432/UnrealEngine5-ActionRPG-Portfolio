// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "BTTask_RandomMove.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class RPG_PORTFOLIO_API UBTTask_RandomMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_RandomMove(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* nodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowAbstract = "true"))
	float search_radius = 1500.0f;

	
};
