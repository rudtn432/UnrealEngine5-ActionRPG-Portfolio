// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class RPG_PORTFOLIO_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "MyEnemy")
	TArray<FName> SpawnEnemyID;

	UPROPERTY(EditAnywhere, Category = "MyEnemy")
	TArray<int32> EnemyInsideBoxNum;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "MyEnemy")
	UStaticMeshComponent* mStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "MyEnemy")
	FVector boxSize;

	UPROPERTY(EditAnywhere, Category = "MyEnemy")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = "MyEnemy")
	TArray<TSubclassOf<AActor>> myEnemy;


private:
	float currentTime;

	bool bSpawn;
};
