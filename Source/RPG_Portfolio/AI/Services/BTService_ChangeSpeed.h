// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RPG_PORTFOLIO_API UBTService_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ChangeSpeed();
	void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	float speed = 400.0f;

protected:
	FString GetStaticServiceDescription() const override;

	
};
