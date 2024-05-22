// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyNPC.generated.h"

UCLASS()
class RPG_PORTFOLIO_API AMyNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNPC();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* NPC_FocusCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree* NPC_Behavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	class USoundCue* InteractSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NPC_Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsTalk;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsShowInteractWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* ActorPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AMyCharacter* Player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
