// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "MyAIController.h"
#include "EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();
	AEnemyCharacter* const npcBot = Cast<AEnemyCharacter>(cont->GetPawn());
	npcBot->GetCharacterMovement()->MaxWalkSpeed = speed;
}

FString UBTService_ChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the NpcBot Speed");
}
