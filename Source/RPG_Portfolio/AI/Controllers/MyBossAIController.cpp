// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Engine.h"
#include "MyCharacter.h"
#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"

AMyBossAIController::AMyBossAIController(FObjectInitializer const& object_initializer)
{
	PrimaryActorTick.bCanEverTick = true;

	behavior_tree_Comp = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	setup_perception_system();
}

void AMyBossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyBossAIController::OnPossess(APawn* _Pawn)
{
	Super::OnPossess(_Pawn);

	ABossCharacter* Boss = Cast<ABossCharacter>(_Pawn);
	if (Boss && Boss->BossBehavior)
	{
		if (Boss->BossBehavior->BlackboardAsset)
		{
			blackboard->InitializeBlackboard(*Boss->BossBehavior->BlackboardAsset);
		}
		behavior_tree_Comp->StartTree(*(Boss->BossBehavior));
	}
}

void AMyBossAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FRotator AMyBossAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

UBlackboardComponent* AMyBossAIController::Get_Blackboard() const
{
	return blackboard;
}

void AMyBossAIController::setup_perception_system()
{
	sight_Config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	sight_Config->SightRadius = 2000.0f;
	sight_Config->LoseSightRadius = sight_Config->SightRadius * 2;
	sight_Config->PeripheralVisionAngleDegrees = 360.0f;
	sight_Config->SetMaxAge(5.0f);
	sight_Config->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	sight_Config->DetectionByAffiliation.bDetectEnemies = true;
	sight_Config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_Config->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*sight_Config->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*sight_Config);
}
