// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
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
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Enemy AI Controller
AMyAIController::AMyAIController(FObjectInitializer const& object_initializer)
{
	PrimaryActorTick.bCanEverTick = true;

	behavior_tree_Comp = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	setup_perception_system();
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AMyAIController::OnPossess(APawn* _Pawn)
{
	Super::OnPossess(_Pawn);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(_Pawn);
	if (Enemy && Enemy->EnemyBehavior)
	{
		if (Enemy->EnemyBehavior->BlackboardAsset)
		{
			blackboard->InitializeBlackboard(*Enemy->EnemyBehavior->BlackboardAsset);
		}
		behavior_tree_Comp->StartTree(*(Enemy->EnemyBehavior));
	}
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FRotator AMyAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AMyAIController::On_Target_Detected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AMyCharacter>(actor))
	{
		if (ch->GetDistanceTo(GetPawn()) <= sight_Config->LoseSightRadius)
		{
			Get_Blackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());

			// 플레이어를 발견하면 시야 각도를 360으로 설정
			sight_Config->PeripheralVisionAngleDegrees = 360.0f;
			GetPerceptionComponent()->ConfigureSense(*sight_Config);
		}
		else	// 시야범위에서 벗어났을 때
		{
			Get_Blackboard()->SetValueAsBool(bb_keys::can_see_player, false);
			// 플레이어를 잃어버리면 시야 각도를 60으로 설정
			sight_Config->PeripheralVisionAngleDegrees = 60.0f;
			GetPerceptionComponent()->ConfigureSense(*sight_Config);
		}
	}
}

UBlackboardComponent* AMyAIController::Get_Blackboard() const
{
	return blackboard;
}

void AMyAIController::setup_perception_system()
{
	sight_Config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	sight_Config->SightRadius = 1000.0f;
	sight_Config->LoseSightRadius = sight_Config->SightRadius * 2;
	sight_Config->PeripheralVisionAngleDegrees = 90.0f;
	sight_Config->SetMaxAge(5.0f);
	sight_Config->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	sight_Config->DetectionByAffiliation.bDetectEnemies = true;
	sight_Config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_Config->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*sight_Config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::On_Target_Detected);
	GetPerceptionComponent()->ConfigureSense(*sight_Config);
}
