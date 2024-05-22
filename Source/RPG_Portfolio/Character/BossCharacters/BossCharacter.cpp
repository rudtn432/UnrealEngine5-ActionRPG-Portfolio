// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Engine.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MyBossAIController.h"
#include "MyCharacter.h"
#include "blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AActor::Tags.Add("Enemy");
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AMyBossAIController* const aiController = Cast<AMyBossAIController>(Controller))
	{
		aiController->Get_Blackboard()->SetValueAsBool(bb_keys::chase_player, true);
	}
}

void ABossCharacter::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void ABossCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	nowHealth = FMath::Min(0, nowHealth);

	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) : GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	StartBossAttTime = false;
	DirToPlayer = false;

	// 시간 느리게 설정
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
	float DeathAnimDuration = PlayAnimMontage(Death_AnimMontage);
	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 50.f), GetActorRotation());
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossCharacter::DeathAnimationEnd, DeathAnimDuration, false);
}

void ABossCharacter::DeathAnimationEnd()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); // 시간 원래대로
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABossCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (nowHealth <= 0)
	{
		return 0.0f;
	}

	if (myGetDamage > 0.f)
	{
		nowHealth -= myGetDamage;
	}

	if (nowHealth <= 0)
	{
		Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		OnHit(myGetDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return myGetDamage;
}

void ABossCharacter::Knockback(FVector KnockbackDirection, float KnockbackForce)
{
	// 넉백 방향을 정규화
	KnockbackDirection.Normalize();

	// 넉백 힘을 적용
	FVector KnockbackImpulse = KnockbackDirection * KnockbackForce;
	GetCharacterMovement()->Launch(KnockbackImpulse);
}

void ABossCharacter::GetEXP(int num)
{
	AMyCharacter* Player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Player->ExpUpdate(num);
}

void ABossCharacter::OnMeleeAttackEnd()
{
	this->isAttacking = false;
	if (AMyBossAIController* const aiController = Cast<AMyBossAIController>(Controller))
	{
		aiController->Get_Blackboard()->SetValueAsBool(bb_keys::chase_player, true);
		aiController->Get_Blackboard()->SetValueAsBool(bb_keys::is_Attacking, false);
	}
}

UAnimMontage* ABossCharacter::Get_Montage() const
{
	return nullptr;
}


