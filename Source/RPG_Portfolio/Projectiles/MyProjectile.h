// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectile.generated.h"

UCLASS()
class RPG_PORTFOLIO_API AMyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float myInitialSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float myMaxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	FName ProjectileName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FORCEINLINE class USphereComponent* GetProjectileCollision() const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
