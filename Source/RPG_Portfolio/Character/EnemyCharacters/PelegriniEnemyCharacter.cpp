// Fill out your copyright notice in the Description page of Project Settings.


#include "PelegriniEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APelegriniEnemyCharacter::APelegriniEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	maxHealth = 60;
	nowHealth = maxHealth;
	enemyID = TEXT("01");

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void APelegriniEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void APelegriniEnemyCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Super::Die(KillingDamage, DamageEvent, Killer, DamageCauser);

	GetEXP(10);
}

// Called every frame
void APelegriniEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APelegriniEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
