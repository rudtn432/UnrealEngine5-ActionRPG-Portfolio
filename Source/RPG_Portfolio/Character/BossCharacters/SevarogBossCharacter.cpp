// Fill out your copyright notice in the Description page of Project Settings.


#include "SevarogBossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyBossAIController.h"
#include "blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

ASevarogBossCharacter::ASevarogBossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	maxHealth = 500;
	nowHealth = maxHealth;
	bossID = TEXT("00");

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASevarogBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	randomTime = FMath::RandRange(2.f, 5.f);
}

void ASevarogBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어 방향으로 회전
	if (DirToPlayer)
	{
		if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
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
		}
	}

	
	if (StartBossAttTime)
	{
		bossAttTime += DeltaTime;
		
		if (bossAttTime >= randomTime)
		{
			// randomTime이 지나면 is_Wandering을 false로 해서 공격패턴 시작
			if (AMyBossAIController* const aiController = Cast<AMyBossAIController>(Controller))
			{
				aiController->Get_Blackboard()->SetValueAsBool(bb_keys::is_Wandering, false);
			}
			randomTime = FMath::RandRange(2.f, 5.f);
			bossAttTime = 0;
			StartBossAttTime = false;
		}
	}
}

void ASevarogBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASevarogBossCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Super::Die(KillingDamage, DamageEvent, Killer, DamageCauser);

	GetEXP(100);
}

int ASevarogBossCharacter::melee_attack_Implementation()
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (AMyBossAIController* const aiController = Cast<AMyBossAIController>(Controller))
		{
			aiController->Get_Blackboard()->SetValueAsBool(bb_keys::is_Attacking, true);
			this->GetCharacterMovement()->StopMovementImmediately();
			this->isAttacking = true;
			this->DirToPlayer = false;
			SelectNextAttack();
		}
	}
	return 0;
}

void ASevarogBossCharacter::SelectNextAttack()
{
	EBossAttackPattern NextAttack = EBossAttackPattern::None;
	do
	{
		int32 AttackIndex = FMath::RandRange(0, 3);		// 0부터 3까지의 무작위 숫자 생성
		NextAttack = static_cast<EBossAttackPattern>(AttackIndex);
	} while (NextAttack == LastAttack);		// 이전 공격과 다를 때까지 반복

	LastAttack = NextAttack;
	switch (NextAttack)
	{
	case EBossAttackPattern::MeleeAttack:
		PerformMeleeAttack();
		break;
	case EBossAttackPattern::TeleportAttack:
		PerformTeleportAttack();
		break;
	case EBossAttackPattern::ProjectileAttack:
		PerformProjectileAttack();
		break;
	case EBossAttackPattern::SlamAttack:
		PerformSlamAttack();
		break;
	default:
		break;
	}
}

// 기본 공격
void ASevarogBossCharacter::PerformMeleeAttack()
{
	OnMeleeAttack1();
}

void ASevarogBossCharacter::OnMeleeAttack1()
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (melee_Attack1_Montage)
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
			PlayAnimMontage(melee_Attack1_Montage);

			AttackMoveDuration = 0.1f; // 이동에 걸리는 시간, 0.1초
			ElapsedTime = 0.0f;
			StartLocation = GetActorLocation();
			TargetLocation = StartLocation + GetActorForwardVector() * 200.f;

			GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &ASevarogBossCharacter::UpdatePosition, AttackMoveDuration / 100.0f, true);

			// 공격 애니메이션의 종료를 감지하는 타이머 설정
			float AttackAnimDuration = melee_Attack1_Montage->GetPlayLength() * (1.f / 0.7f) - 0.3f;
			GetWorldTimerManager().SetTimer(AttackAnimEndTimerHandle, this, &ASevarogBossCharacter::OnMeleeAttack2, AttackAnimDuration, false);
		}
	}
}

void ASevarogBossCharacter::OnMeleeAttack2()
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (melee_Attack2_Montage)
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

			PlayAnimMontage(melee_Attack2_Montage);

			AttackMoveDuration = 0.1f; // 이동에 걸리는 시간, 0.1초
			ElapsedTime = 0.0f;
			StartLocation = GetActorLocation();
			TargetLocation = StartLocation + GetActorForwardVector() * 200.f;

			GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &ASevarogBossCharacter::UpdatePosition, AttackMoveDuration / 100.0f, true);

			// 공격 애니메이션의 종료를 감지하는 타이머 설정
			float AttackAnimDuration = melee_Attack2_Montage->GetPlayLength() * (1.f / 0.7f) - 0.1f;
			GetWorldTimerManager().SetTimer(AttackAnimEndTimerHandle, this, &ASevarogBossCharacter::OnMeleeAttackEnd, AttackAnimDuration, false);
		}
	}
}

void ASevarogBossCharacter::UpdatePosition()
{
	ElapsedTime += AttackMoveDuration / 100.0f;
	if (ElapsedTime >= AttackMoveDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
	}

	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, ElapsedTime / AttackMoveDuration);
	SetActorLocation(NewLocation);
}

// 플레이어 뒤로 텔레포트 후 공격
void ASevarogBossCharacter::PerformTeleportAttack()
{
	// 보스를 일시적으로 숨기고, 데미지를 받지 않도록 충돌을 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (HideParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HideParticle, GetActorLocation(), GetActorRotation());
	}

	// 랜덤 시간 후에 보스를 다시 보이게 하고 공격
	float Delay = FMath::RandRange(3.0f, 7.0f);
	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ASevarogBossCharacter::AppearAndAttack, Delay, false);
}

void ASevarogBossCharacter::AppearAndAttack()
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// 플레이어의 현재 위치와 방향을 얻음
		FVector PlayerLocation = player->GetActorLocation();
		FVector PlayerForwardVector = player->GetActorForwardVector();

		// 플레이어가 바라보는 반대 방향에 보스를 나타내기
		float DistanceFromPlayer = 500.0f;		// 플레이어로부터 얼마나 떨어뜨릴지 결정하는 거리
		FVector NewBossLocation = FVector(PlayerLocation.X - (PlayerForwardVector.X * DistanceFromPlayer),
			PlayerLocation.Y - (PlayerForwardVector.Y * DistanceFromPlayer),
			this->GetActorLocation().Z);
		SetActorLocation(NewBossLocation);
	}

	// 보스를 다시 보이기
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	if (AppearParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AppearParticle, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 250.f), GetActorRotation());
	}

	OnMeleeAttack1();
}

void ASevarogBossCharacter::PerformProjectileAttack()
{
	// 보스를 일시적으로 숨기고, 데미지를 받지 않도록 충돌을 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (HideParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HideParticle, GetActorLocation(), GetActorRotation());
	}

	// 1초 후에 보스를 다시 보이게 하고 Projectile 생성
	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ASevarogBossCharacter::AppearAndCreateProjectile, 1.f, false);
}

void ASevarogBossCharacter::AppearAndCreateProjectile()
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// 정해진 위치에 보스를 나타나기
		FVector NewBossLocation = FVector(-48060.0f, 15630.0f, GetActorLocation().Z);
		SetActorLocation(NewBossLocation);
		SetActorRotation(FQuat(0));
	}
	if (AppearParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AppearParticle, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 250.f), GetActorRotation());
	}

	// 보스를 다시 보이기
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (createProjectileAndShoot_Montage)
	{
		PlayAnimMontage(createProjectileAndShoot_Montage);

		// 공격 애니메이션의 종료를 감지하는 타이머 설정
		float AttackAnimDuration = createProjectileAndShoot_Montage->GetPlayLength();
		GetWorldTimerManager().SetTimer(AttackAnimEndTimerHandle, this, &ASevarogBossCharacter::OnMeleeAttackEnd, AttackAnimDuration, false);
	}
}

void ASevarogBossCharacter::PerformSlamAttack()
{
	if (ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (slam_Montage)
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
				NewRotation.Yaw -= 30.0f;
				// 적의 회전을 플레이어를 바라보도록 설정
				SetActorRotation(NewRotation);
			}
			PlayAnimMontage(slam_Montage);

			// 공격 애니메이션의 종료를 감지하는 타이머 설정
			float AttackAnimDuration = slam_Montage->GetPlayLength();
			GetWorldTimerManager().SetTimer(AttackAnimEndTimerHandle, this, &ASevarogBossCharacter::OnMeleeAttackEnd, AttackAnimDuration, false);
		}
	}
}
