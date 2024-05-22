// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPC.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "MyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

// Sets default values
AMyNPC::AMyNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	NPC_FocusCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	NPC_FocusCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	NPC_FocusCamera->bUsePawnControlRotation = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GateCollisionSphere"));
	CollisionSphere->SetupAttachment(this->GetCapsuleComponent());
	CollisionSphere->InitSphereRadius(250.f);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyNPC::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AMyNPC::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AMyNPC::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void AMyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsShowInteractWidget && OtherActor->ActorHasTag("Player"))
	{
		bIsShowInteractWidget = true;
		ActorPlayer = OtherActor;
		Player->InteractBeginOverlap();


	}
	/*if (OtherActor && OtherActor != this && !bIsTalk)
	{
		if (InteractSound->IsPlayable())
		{
			UGameplayStatics::PlaySound2D(this, InteractSound);
		}
		bIsTalk = true;
	}*/
}

void AMyNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsShowInteractWidget && OtherActor->ActorHasTag("Player"))
	{
		bIsShowInteractWidget = false;
		Player->InteractEndOverlap();
	}
}

