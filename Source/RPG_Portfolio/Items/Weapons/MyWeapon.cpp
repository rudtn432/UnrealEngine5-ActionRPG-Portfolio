// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AMyWeapon::AMyWeapon(const class FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetBoxExtent(FVector(5.f, 5.f, 5.f));
	WeaponCollision->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "DamageSocket");
	
}

//-----------------------ChangeWeapon------------------------------
void AMyWeapon::SetOwningPawn(AMyCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		MyPawn = NewOwner;
	}
}

void AMyWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		USkeletalMeshComponent* PawnMesh = MyPawn->GetSpesificPawnMesh();
		FName AttachPoint = MyPawn->GetWeaponAttachPoint();
		WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
	}
}

void AMyWeapon::OnEquip(const AMyWeapon* LastWeapon)
{
	AttachMeshToPawn();
	WeaponMesh->SetHiddenInGame(false);
}

void AMyWeapon::OnUnEquip()
{
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	WeaponMesh->SetHiddenInGame(true);
}
//-----------------------------------------------------------------


// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


