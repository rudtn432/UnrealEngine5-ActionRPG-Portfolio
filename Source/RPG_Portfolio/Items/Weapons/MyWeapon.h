// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyCharacter.h"
#include "MyWeapon.generated.h"

// 무기의 손잡이 유형을 나타내는 열거형
UENUM(BlueprintType)
enum class EWeaponHandType : uint8
{
	OneHand   UMETA(DisplayName = "One Hand"),
	TwoHand   UMETA(DisplayName = "Two Hand")
};

UCLASS()
class RPG_PORTFOLIO_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon(const class FObjectInitializer& ObjectInitializer);

	void SetOwningPawn(AMyCharacter* NewOwner);
	void AttachMeshToPawn();
	void OnEquip(const AMyWeapon* LastWeapon);
	void OnUnEquip();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UBoxComponent* WeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponHandType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int64 minAttackDmg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int64 maxAttackDmg;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;
	

protected:
	class AMyCharacter* MyPawn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
