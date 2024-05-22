// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "Animation/AnimMontage.h"
#include "MyCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class RPG_PORTFOLIO_API AEnemyCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
	FName enemyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
	FText enemyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 nowHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 maxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	virtual void DeathAnimationEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree* EnemyBehavior;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* BeHit_AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Death_AnimMontage;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	virtual void Knockback(FVector KnockbackDirection, float KnockbackForce);

	UFUNCTION(BlueprintCallable)
	void GetEXP(int num);

	int melee_attack_Implementation();
	virtual void OnMeleeAttackEnd();
	UFUNCTION(BlueprintCallable)
	virtual void BeHitEnd();

	UAnimMontage* Get_Montage() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	UAnimMontage* WeakAttack_Montage;

	
};
