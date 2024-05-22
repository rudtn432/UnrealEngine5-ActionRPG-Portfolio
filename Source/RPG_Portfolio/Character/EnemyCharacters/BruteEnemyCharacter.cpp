// Fill out your copyright notice in the Description page of Project Settings.


#include "BruteEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABruteEnemyCharacter::ABruteEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	maxHealth = 100;
	nowHealth = maxHealth;
	enemyID = TEXT("00");
	enemyName = FText::FromString(TEXT("브루트"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ABruteEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABruteEnemyCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Super::Die(KillingDamage, DamageEvent, Killer, DamageCauser);

	GetEXP(10);
}

// Called every frame
void ABruteEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABruteEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

