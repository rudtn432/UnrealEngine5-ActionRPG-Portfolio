// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Engine.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AActor::Tags.Add("Enemy");
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (AMyAIController* const aiController = Cast<AMyAIController>(Controller))
	{
		aiController->Get_Blackboard()->SetValueAsBool(bb_keys::can_see_player, true);
		aiController->Get_Blackboard()->SetValueAsBool(bb_keys::be_hit, true);
	}
	
	PlayAnimMontage(BeHit_AnimMontage);
	this->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	this->GetCharacterMovement()->StopMovementImmediately();
	this->Knockback(DamageCauser->GetActorForwardVector(), 600.f);
	this->isAttacking = false;

	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void AEnemyCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
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

	float DeathAnimDuration = PlayAnimMontage(Death_AnimMontage);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacter::DeathAnimationEnd, DeathAnimDuration, false);
}

void AEnemyCharacter::GetEXP(int num)
{
	AMyCharacter* Player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Player->ExpUpdate(num);
}

void AEnemyCharacter::DeathAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void AEnemyCharacter::Knockback(FVector KnockbackDirection, float KnockbackForce)
{
	// 넉백 방향을 정규화
	KnockbackDirection.Normalize();

	// 넉백 힘을 적용
	FVector KnockbackImpulse = KnockbackDirection * KnockbackForce;
	GetCharacterMovement()->Launch(KnockbackImpulse);
}

int AEnemyCharacter::melee_attack_Implementation()
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (!this->isAttacking)
		{
			if (WeakAttack_Montage)
			{
				// 플레이어 방향으로 공격
				FVector DirectionToPlayer = player->GetActorLocation() - GetActorLocation();
				DirectionToPlayer.Z = 0.0f;		// 수직 성분 무시
				// 방향 벡터가 거의 0이 아닌지 확인하여 정규화
				if (!DirectionToPlayer.IsNearlyZero())
				{
					// 방향 벡터 정규화
					DirectionToPlayer.Normalize();
					// 플레이어를 바라보도록 새로운 회전 계산
					FRotator NewRotation = DirectionToPlayer.Rotation();
					// 적의 회전을 플레이어를 바라보도록 설정
					SetActorRotation(NewRotation);
				}

				this->isAttacking = true;
				PlayAnimMontage(WeakAttack_Montage);

				// 공격 중일 때 이동을 중지
				if (GetCharacterMovement())
				{
					GetCharacterMovement()->DisableMovement();
				}

				// 공격 애니메이션의 종료를 감지하는 타이머 설정
				FTimerHandle AttackAnimEndTimerHandle;
				float AttackAnimDuration = WeakAttack_Montage->GetPlayLength() + 0.1f;
				GetWorldTimerManager().SetTimer(AttackAnimEndTimerHandle, this, &AEnemyCharacter::OnMeleeAttackEnd, AttackAnimDuration, false);
			}
		}
	}
	else
	{
		if (AMyAIController* const aiController = Cast<AMyAIController>(Controller))
		{
			aiController->Get_Blackboard()->SetValueAsBool(bb_keys::can_see_player, false);
		}
	}
	
	return 0;
}

void AEnemyCharacter::OnMeleeAttackEnd()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (!AnimInstance->Montage_IsPlaying(WeakAttack_Montage))
		{
			this->isAttacking = false;

			// 이동 다시 활성화
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
		}
	}
}

// BeHitMontage가 끝나면 BP에서 N초 뒤에 함수 호출
void AEnemyCharacter::BeHitEnd()
{
	AMyAIController* const aiController = Cast<AMyAIController>(Controller);

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		// N초 뒤에 호출 했는데 또 데미지를 입어서 be_hit가 풀리는 현상 방지 
		if (aiController && !AnimInstance->Montage_IsPlaying(BeHit_AnimMontage))
		{
			aiController->Get_Blackboard()->SetValueAsBool(bb_keys::be_hit, false);
			this->isAttacking = false;
		}
	}
}

UAnimMontage* AEnemyCharacter::Get_Montage() const
{
	return nullptr;
}

