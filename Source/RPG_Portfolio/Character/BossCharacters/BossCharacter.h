// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "BossCharacter.generated.h"

UCLASS()
class RPG_PORTFOLIO_API ABossCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
	FName bossID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
	FText bossName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 nowHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	int32 maxHealth;

	bool StartBossAttTime;
	bool DirToPlayer;
	float bossAttTime;
	float randomTime;

protected:
	virtual void BeginPlay() override;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	virtual void DeathAnimationEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree* BossBehavior;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Death_AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* DeathParticle;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	virtual void Knockback(FVector KnockbackDirection, float KnockbackForce);

	UFUNCTION(BlueprintCallable)
	void GetEXP(int num);
	
	virtual void OnMeleeAttackEnd();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* Get_Montage() const;
};
