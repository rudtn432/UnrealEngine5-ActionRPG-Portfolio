// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MyCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterInfo :public FTableRowBase
{
	GENERATED_BODY()
public:
	FCharacterInfo()
	{
		CharacterName = FText::FromString("Name");
		CharacterLevel = 1;
		CharacterMaxEXP = 100;
		CharacterNowEXP = 0;
		CharacterMaxHP = 100;
		CharacterNowHP = 100;
		CharacterGold = 0;
	}

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterMaxEXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterNowEXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterNowHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 CharacterGold;
};

UCLASS()
class RPG_PORTFOLIO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* FollowCamera;

	virtual void PostInitializeComponents();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool beHit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	FName myID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 myNowHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 myMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 myNowEXP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 myMaxEXP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float myStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
	int64 myGold;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 myLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int64 myMinAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int64 myMaxAttackDamage;
	// myWeaponType 0: 한손, 1: 두손
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 myWeaponType;

	FCharacterInfo* MyCharacterInfo;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FName WeaponAttachPoint;

	TArray<class AMyWeapon*> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class AMyWeapon* CurrentWeapon;

	void AddWeapon(class AMyWeapon* Weapon);

	void SetCurrentWeapon(class AMyWeapon* NewWeapon, class AMyWeapon* LastWeapon);

	void SpawnDefaultInventory();

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	void DeathAnimationEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<TSubclassOf<class AMyWeapon>> WeaponClasses;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> uiInventoryBPClass;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* uiInventoryWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> uiInteractBPClass;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* uiInteractWidget;

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float value);
	void MoveRight(float value);
	// 키를 누르면 점프 플래그를 설정합니다.
	UFUNCTION()
	void StartJump();
	// 키를 떼면 점프 플래그를 지웁니다.
	UFUNCTION()
	void StopJump();

	// 인벤토리 //
	UFUNCTION()
	void ShowInventory();
	UFUNCTION(BlueprintCallable)
	void CloseInventory();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsShowInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	FVector2D InventoryPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	int32 InventoryIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	bool bIsShowInteractWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyState")
	bool bIsRolling;

	void RecoverStamina(float DeltaTime);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyState")
	bool bIsFullStamina;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* BeHit_AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Death_AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Roll_AnimMontage;
	
	//PlayerItem Array------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int32> MyItemArray_Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int32> MyItemArray_Use;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int32> MyItemArray_UseQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int32> MyItemArray_Etc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int32> MyItemArray_EtcQuantity;
	//----------------------------------------------------------------------
	
	UFUNCTION(BlueprintCallable)
	void HealthUpdate(int32 value);
	UFUNCTION(BlueprintCallable)
	void ExpUpdate(int32 value);

	//ShopItem Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> ShopItemArray_ch;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	void Knockback(FVector KnockbackDirection, float KnockbackForce);

	UFUNCTION(BlueprintCallable)
	void BeHitEnd();

	UFUNCTION(BlueprintCallable)
	void UseItem(FName itemID);

	UPROPERTY(EditDefaultsOnly, Category = "MyFX")
	UParticleSystem* HitFX;

	void ShowFX();

	//------------Change Weapon-----------
	USkeletalMeshComponent* GetSpesificPawnMesh() const;

	FName GetWeaponAttachPoint() const;

	void EquipWeapon(class AMyWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void OnChangeWeapon(int32 _itemID);
	//------------------------------------

	UFUNCTION(BlueprintCallable)
	void InteractBeginOverlap();
	UFUNCTION(BlueprintCallable)
	void InteractEndOverlap();
	void Interact();

private:
	class UAIPerceptionStimuliSourceComponent* stimulus;
	void Setup_Stimulus();

	void Roll();
	void RollEnd();
};
