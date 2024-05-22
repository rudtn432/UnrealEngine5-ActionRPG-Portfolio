// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyWeapon.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "Engine.h"
#include "Perception/AISense_Sight.h"
#include "Misc/OutputDeviceNull.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InteractInterface.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	myID = "1";
	WeaponAttachPoint = FName(TEXT("HandWeaponSocket"));
	myMaxHealth = 100;
	myNowHealth = myMaxHealth;
	myMaxEXP = 100;

	Setup_Stimulus();

	if (UDataTable* DT_CharacterInfo = LoadObject<UDataTable>(nullptr, TEXT("/Game/MainFolder/Data/CharacterInfo_DT")))
	{
		// 데이터 테이블에 있는 모든 이름 검색
		TArray<FName> RowNames = DT_CharacterInfo->GetRowNames();
		for (int i = 0; i < RowNames.Num(); ++i)
		{
			// CharacterInfo를 포인터로 저장
			MyCharacterInfo = DT_CharacterInfo->FindRow<FCharacterInfo>(RowNames[i], RowNames[i].ToString());
			if (MyCharacterInfo && MyCharacterInfo->CharacterID == myID)
			{
				// 필요할 때 MyCharacterInfo 값 사용
				MyCharacterInfo->CharacterNowHP = myNowHealth;
				myNowEXP = MyCharacterInfo->CharacterNowEXP;
				myLevel = MyCharacterInfo->CharacterLevel;
				break;
			}
			MyCharacterInfo = NULL;
		}
	}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();
}


//////////////////////////////////////////////////////////////////////
// Weapon

USkeletalMeshComponent* AMyCharacter::GetSpesificPawnMesh() const
{
	return GetMesh();
}

FName AMyCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

void AMyCharacter::EquipWeapon(AMyWeapon* Weapon)
{
	if (Weapon)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
}

void AMyCharacter::OnChangeWeapon(int32 _itemID)
{
	if (Weapons.Num() > 0)
	{
		EquipWeapon(Weapons[_itemID]);
	}
}

void AMyCharacter::AddWeapon(AMyWeapon* Weapon)
{
	if (Weapon)
	{
		Weapons.AddUnique(Weapon);
	}
}

void AMyCharacter::SetCurrentWeapon(AMyWeapon* NewWeapon, AMyWeapon* LastWeapon)
{
	AMyWeapon* LocalLastWeapon = NULL;
	if (LastWeapon != NULL)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);
		NewWeapon->OnEquip(LastWeapon);
		myMinAttackDamage = NewWeapon->minAttackDmg;
		myMaxAttackDamage = NewWeapon->maxAttackDmg;
		switch (NewWeapon->WeaponType)
		{
		case EWeaponHandType::OneHand:
			myWeaponType = 0;
			break;
		case EWeaponHandType::TwoHand:
			myWeaponType = 1;
			break;
		default:
			myWeaponType = -1;
			break;
		}
	}
}
void AMyCharacter::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = WeaponClasses.Num();
	if (NumWeaponClasses > 0)
	{
		for (int32 i = 0; i < NumWeaponClasses; i++)
		{
			FActorSpawnParameters SpawnInfo;
			UWorld* World = GetWorld();
			if (World)
			{
				AMyWeapon* NewWeapon = World->SpawnActor<AMyWeapon>(WeaponClasses[i], SpawnInfo);
				if (NewWeapon)
				{
					AddWeapon(NewWeapon);
				}
			}
		}
		if (Weapons.Num() > 0)
		{
			EquipWeapon(Weapons[0]);
			myMinAttackDamage = Weapons[0]->minAttackDmg;
			myMaxAttackDamage = Weapons[0]->maxAttackDmg;
		}
	}
}
//////////////////////////////////////////////////////////////////////


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultInventory();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!beHit && !bIsRolling)
		RecoverStamina(DeltaTime);
}

void AMyCharacter::RecoverStamina(float DeltaTime)
{
	// 현재 스태미너가 최대 스태미너 이하일 경우에만 회복
	if (!bIsFullStamina)
	{
		// 1초당 회복할 스태미너 양
		float StaminaRecoveryRate = 5.0f;

		// 스태미너 회복량 = 회복률 * DeltaTime
		float StaminaRecoveryAmount = StaminaRecoveryRate * DeltaTime;

		// 현재 스태미너에 회복량 추가
		myStamina = FMath::Min(100.f, myStamina + StaminaRecoveryAmount);
	}
	else if (myStamina >= 100.f)
		bIsFullStamina = true;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJump);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyCharacter::ShowInventory);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyCharacter::Interact);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMyCharacter::Roll);

}

float AMyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsRolling)
	{
		if (myNowHealth <= 0)
		{
			return 0.0f;
		}

		if (myGetDamage > 0.f)
		{
			HealthUpdate(-myGetDamage);
		}

		if (myNowHealth <= 0)
		{
			Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			OnHit(myGetDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}
	}

	return myGetDamage;
}

void AMyCharacter::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	PlayAnimMontage(BeHit_AnimMontage);
	this->GetCharacterMovement()->StopMovementImmediately();
	this->Knockback(DamageCauser->GetActorForwardVector(), 500.f);
	if (myStamina <= 100.f)
		myStamina = FMath::Min(100.f, myStamina + 5.f);
	beHit = true;
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void AMyCharacter::Knockback(FVector KnockbackDirection, float KnockbackForce)
{
	// 넉백 방향을 정규화
	KnockbackDirection.Normalize();

	// 넉백 힘을 적용
	FVector KnockbackImpulse = KnockbackDirection * KnockbackForce;
	GetCharacterMovement()->Launch(KnockbackImpulse);
}

void AMyCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	myNowHealth = FMath::Min(0, myNowHealth);

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
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyCharacter::DeathAnimationEnd, DeathAnimDuration, false);
}

void AMyCharacter::DeathAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);

	// BP_MyCharacter에 있는 함수 호출
	FOutputDeviceNull _null;
	CallFunctionByNameWithArguments(TEXT("CreateWidgetGameOver"), _null, nullptr, true);
}

void AMyCharacter::BeHitEnd()
{
	bIsAttacking = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 0.N초 뒤에 호출 했는데 또 데미지를 입어서 behit가 풀리는 현상 방지
		if (!AnimInstance->Montage_IsPlaying(BeHit_AnimMontage))
		{
			beHit = false;
		}
	}
}

void AMyCharacter::ShowFX()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation());
}

void AMyCharacter::Setup_Stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AMyCharacter::HealthUpdate(int32 value)
{
	myNowHealth += value;
	if (myNowHealth >= myMaxHealth)
		myNowHealth = myMaxHealth;
	if (myNowHealth <= 0)
		myNowHealth = 0;

	if (MyCharacterInfo)
	{
		// 가져온 HP 값을 사용하여 CharacterNowHP를 업데이트
		MyCharacterInfo->CharacterNowHP = myNowHealth;
	}
}

void AMyCharacter::UseItem(FName itemID)
{
	if (itemID == TEXT("100"))
		HealthUpdate(10);
	else if (itemID == TEXT("101"))
		HealthUpdate(20);
}

void AMyCharacter::InteractBeginOverlap()
{
	if (uiInteractBPClass)
	{
		if (!bIsShowInteractWidget)
		{
			// Interact Widget 생성 및 표시
			uiInteractWidget = CreateWidget<UUserWidget>(GetWorld(), uiInteractBPClass);
			if (uiInteractWidget)
			{
				uiInteractWidget->AddToViewport();
				bIsShowInteractWidget = true;
			}
		}
	}
}

void AMyCharacter::InteractEndOverlap()
{
	if (uiInteractBPClass)
	{
		if (bIsShowInteractWidget)
		{
			if (uiInteractWidget)
			{
				uiInteractWidget->RemoveFromParent();
				bIsShowInteractWidget = false;
			}
		}
	}
}

void AMyCharacter::Interact()
{
	TArray<AActor*> OverlappingActors;
	// 플레이어와 닿은 Actor들 검색
	GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		// Actor들을 돌아가면서 InteractInterface의 인터페이스를 구현하고 있는지 확인
		if (UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_Interact(Actor);
			break;
		}
	}

}

void AMyCharacter::ExpUpdate(int32 value)
{
	int sumEXP = myNowEXP + value;

	if (sumEXP >= myMaxEXP)
	{
		myNowEXP = sumEXP - myMaxEXP;
		myLevel += 1;
	}
	else
	{
		myNowEXP += value;
	}

	if (MyCharacterInfo)
	{
		MyCharacterInfo->CharacterNowEXP = myNowEXP;
		MyCharacterInfo->CharacterMaxEXP = myMaxEXP;
		MyCharacterInfo->CharacterLevel = myLevel;
	}
}

void AMyCharacter::Roll()
{
	if (!bIsRolling && !beHit && !GetCharacterMovement()->IsFalling() && myStamina >= 30.f)
	{
		myStamina -= 30.f;
		bIsFullStamina = false;
		bIsRolling = true;
		bIsAttacking = false;
		// 0.0001초 전에 GetLastInputVector가 (0,0,0)이 아니라면
		if (!GetCharacterMovement()->GetLastInputVector().IsNearlyZero(0.0001f))
		{
			// 마지막 입력 벡터 가져오기
			FVector LastInputVector = GetCharacterMovement()->GetLastInputVector();
			// 벡터의 방향을 기반으로 회전 생성
			FRotator NewRotation = FRotationMatrix::MakeFromX(LastInputVector).Rotator();
			// 플레이어의 방향 설정
			SetActorRotation(NewRotation);
		}

		float RollAnimDuration = PlayAnimMontage(Roll_AnimMontage);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyCharacter::RollEnd, (RollAnimDuration / 1.5f) - 0.05f, false);
	}
}

void AMyCharacter::RollEnd()
{
	if (bIsRolling)
	{
		bIsRolling = false;
	}
}

void AMyCharacter::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && !beHit)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AMyCharacter::MoveRight(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && !beHit)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void AMyCharacter::StartJump()
{
	bPressedJump = true;
}

void AMyCharacter::StopJump()
{
	bPressedJump = false;
}

void AMyCharacter::ShowInventory()
{
	if (uiInventoryBPClass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (!bIsShowInventory)
		{
			if (PlayerController)
			{
				// 마우스 커서 보이기
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeGameAndUI());
			}
			// Inventory Widget 생성 및 표시
			uiInventoryWidget = CreateWidget<UUserWidget>(GetWorld(), uiInventoryBPClass);
			if (uiInventoryWidget)
			{
				uiInventoryWidget->AddToViewport();
				bIsShowInventory = true;
			}
		}
		else
		{
			CloseInventory();
		}
	}
}

void AMyCharacter::CloseInventory()
{
	if (uiInventoryBPClass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			// 마우스 커서 숨기기
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
		// Inventory Widget 숨기기
		if (uiInventoryWidget)
		{
			uiInventoryWidget->RemoveFromParent();
			bIsShowInventory = false;
		}
	}
}

