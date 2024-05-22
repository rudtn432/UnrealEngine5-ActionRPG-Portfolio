// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Engine.h"
#include "EnemyCharacter.h"
#include "MyCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bSpawn = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnPointComp"));
	CollisionBox->InitBoxExtent(boxSize);
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawnPoint::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASpawnPoint::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->InitBoxExtent(boxSize);
	//DrawDebugBox(GetWorld(), GetActorLocation(), boxSize, FColor::Green, true, -1, 0, 2);
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpawn)
	{
		currentTime += 1 * DeltaTime;
		if (currentTime >= 10.0f)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();

			UWorld* WRLD = GetWorld();

			FVector location = GetActorLocation();
			FRotator rotation = GetActorRotation();

			// 박스 안에 있는 Enemy의 수를 저장할 변수 초기화
			for (int i = 0; i < EnemyInsideBoxNum.Num(); i++)
			{
				EnemyInsideBoxNum[i] = 0;
			}

			// 박스와 겹치는 모든 액터를 검색
			TArray<AActor*> OverlappingActors;
			GetOverlappingActors(OverlappingActors, AEnemyCharacter::StaticClass());

			// 각 액터를 검사하여 Enemy인지 확인하고, 박스 안에 있는지 확인
			for (AActor* Actor : OverlappingActors)
			{
				// 액터가 AEnemyCharacter 클래스의 인스턴스인지 확인
				AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
				if (EnemyCharacter)
				{
					// 액터의 위치를 가져옴
					FVector ActorLocation = Actor->GetActorLocation();
					// 액터의 위치가 박스 안에 있는지 확인
					if (ActorLocation.X >= location.X - boxSize.X &&
						ActorLocation.X <= location.X + boxSize.X &&
						ActorLocation.Y >= location.Y - boxSize.Y &&
						ActorLocation.Y <= location.Y + boxSize.Y &&
						ActorLocation.Z >= location.Z - boxSize.Z &&
						ActorLocation.Z <= location.Z + boxSize.Z &&
						EnemyCharacter->enemyID == SpawnEnemyID[0])
					{
						// 박스 안에 있는 Enemy의 수를 증가
						EnemyInsideBoxNum[0]++;
					}
					else if (ActorLocation.X >= location.X - boxSize.X &&
						ActorLocation.X <= location.X + boxSize.X &&
						ActorLocation.Y >= location.Y - boxSize.Y &&
						ActorLocation.Y <= location.Y + boxSize.Y &&
						ActorLocation.Z >= location.Z - boxSize.Z &&
						ActorLocation.Z <= location.Z + boxSize.Z &&
						EnemyCharacter->enemyID == SpawnEnemyID[1])
					{
						// 박스 안에 있는 Enemy의 수를 증가
						EnemyInsideBoxNum[1]++;
					}
				}
			}
			if (myEnemy[0])
			{
				// 박스 안의 Enemy 수가 N명보다 적으면 N명까지 Spawn
				for (int i = 0; i < 6 - EnemyInsideBoxNum[0]; i++)
				{
					// 박스 사이즈 안쪽 랜덤 위치 받기
					FVector RandomLocation = FMath::RandPointInBox(FBox(location - boxSize, location + boxSize));
					// Z축은 기본 location 위치로
					FVector SpawnLocation = FVector(RandomLocation.X, RandomLocation.Y, location.Z);
					AEnemyCharacter* myNewEnemy = WRLD->SpawnActor<AEnemyCharacter>(myEnemy[0], SpawnLocation, rotation, spawnParams);
				}
			}
			if (myEnemy[1])
			{
				// 박스 안의 Enemy 수가 N명보다 적으면 N명까지 Spawn
				for (int i = 0; i < 3 - EnemyInsideBoxNum[1]; i++)
				{
					// 박스 사이즈 안쪽 랜덤 위치 받기
					FVector RandomLocation = FMath::RandPointInBox(FBox(location - boxSize, location + boxSize));
					// Z축은 기본 location 위치로
					FVector SpawnLocation = FVector(RandomLocation.X, RandomLocation.Y, location.Z);
					AEnemyCharacter* myNewEnemy = WRLD->SpawnActor<AEnemyCharacter>(myEnemy[1], SpawnLocation, rotation, spawnParams);
				}
			}

			currentTime = 0.0f;
		}
	}
}

void ASpawnPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMyCharacter::StaticClass()))
	{
		if (!bSpawn)
		{
			bSpawn = true;
		}
	}
}

void ASpawnPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AMyCharacter::StaticClass()))
	{
		if (bSpawn)
		{
			bSpawn = false;
		}
	}
}

