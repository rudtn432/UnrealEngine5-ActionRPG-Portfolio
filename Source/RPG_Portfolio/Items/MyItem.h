// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "MyItem.generated.h"

USTRUCT(BlueprintType)
struct FItemInform :public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemInform()
	{
		ItemName = FText::FromString("ItemName");
		ItemDescription = FText::FromString("This item is ...");
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;

	// ItemType : 장비 0, 소비 1, 기타 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> itemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescription;

};

UCLASS()
class RPG_PORTFOLIO_API AMyItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
