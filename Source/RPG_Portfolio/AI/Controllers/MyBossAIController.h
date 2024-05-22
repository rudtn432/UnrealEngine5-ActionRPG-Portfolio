// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyBossAIController.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS(Config = Game)
class RPG_PORTFOLIO_API AMyBossAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyBossAIController(FObjectInitializer const& object_initializer);
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* _Pawn) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	class UBehaviorTreeComponent* behavior_tree_Comp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	class UBehaviorTree* bTree;

	class UBlackboardComponent* Get_Blackboard() const;

	class UAISenseConfig_Sight* sight_Config;

protected:


private:


	class UBlackboardComponent* blackboard;

	void setup_perception_system();
};
