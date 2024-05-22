// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_PortfolioGameMode.h"
#include "RPG_PortfolioCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPG_PortfolioGameMode::ARPG_PortfolioGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		//DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
