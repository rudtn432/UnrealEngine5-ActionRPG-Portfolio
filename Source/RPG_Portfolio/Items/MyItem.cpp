// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "MyCharacter.h"
#include "Engine.h"

// Sets default values
AMyItem::AMyItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

