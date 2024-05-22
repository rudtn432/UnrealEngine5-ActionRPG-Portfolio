// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "BruteEnemyCharacter.generated.h"

UCLASS()
class RPG_PORTFOLIO_API ABruteEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABruteEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
