// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossCharacter.h"
#include "SevarogBossCharacter.generated.h"

/**
 * 
 */

enum class EBossAttackPattern
{
	MeleeAttack,			// 기본 공격
	TeleportAttack,			// 플레이어 뒤로 순간이동 후 공격
	ProjectileAttack,		// 발사체 생성 후 발사
	SlamAttack,				// 망치 내려치기
	None
};

UCLASS()
class RPG_PORTFOLIO_API ASevarogBossCharacter : public ABossCharacter
{
	GENERATED_BODY()
public:
	ASevarogBossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) override;

	EBossAttackPattern LastAttack = EBossAttackPattern::None;

	void PerformMeleeAttack();
	void OnMeleeAttack1();
	void OnMeleeAttack2();

	void PerformTeleportAttack();
	void AppearAndAttack();

	void PerformProjectileAttack();
	void AppearAndCreateProjectile();

	void PerformSlamAttack();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SelectNextAttack();

	int melee_attack_Implementation();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	UAnimMontage* melee_Attack1_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	UAnimMontage* melee_Attack2_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	UAnimMontage* slam_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowAbstract = "true"))
	UAnimMontage* createProjectileAndShoot_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* HideParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* AppearParticle;


private:
	FTimerHandle AttackAnimEndTimerHandle;
	FVector StartLocation;
	FVector TargetLocation;
	FTimerHandle MoveTimerHandle;
	float AttackMoveDuration;
	float ElapsedTime;

	// 실제로 위치를 업데이트하는 함수
	void UpdatePosition();

};
