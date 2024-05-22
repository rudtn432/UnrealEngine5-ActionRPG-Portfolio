// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossWandering.h"
#include "blackboard_keys.h"
#include "MyBossAIController.h"
#include "BossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BossWandering::UBTTask_BossWandering(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Wandering Player");
}

EBTNodeResult::Type UBTTask_BossWandering::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = Cast<AMyBossAIController>(owner_comp.GetAIOwner());
	ABossCharacter* const boss = Cast<ABossCharacter>(cont->GetPawn());

	if (cont->Get_Blackboard()->GetValueAsBool(bb_keys::is_Wandering))
	{
		// 보스 주위 어슬렁거리기
		if (boss)
		{
			if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				FVector const player_loc = player->GetActorLocation();
				FVector const boss_loc = boss->GetActorLocation();
				FNavLocation loc;

				// 500 ~ 700 반경 내에서 랜덤한 위치 찾기
				UNavigationSystemV1* const navSys = UNavigationSystemV1::GetCurrent(GetWorld());
				for (int i = 0; i < 100; i++)
				{
					if (navSys && navSys->GetRandomPointInNavigableRadius(player_loc, 500.0f, loc, nullptr))
					{
						float Distance = FVector::Dist(player_loc, loc.Location);
						float bossDistance = FVector::Dist(boss_loc, loc.Location);
						if (Distance >= 300.f && Distance <= 500.f && bossDistance >= 300.f && bossDistance <= 400.f)
						{
							cont->Get_Blackboard()->SetValueAsVector(bb_keys::random_loc_vector, loc.Location);
							boss->StartBossAttTime = true;
							boss->DirToPlayer = true;
							break;
						}
					}
				}
			}
		}
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
