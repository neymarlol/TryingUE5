// Copyright Epic Games, Inc. All Rights Reserved.

#include "TryingUE5GameMode.h"
#include "TryingUE5Character.h"
#include "UObject/ConstructorHelpers.h"

ATryingUE5GameMode::ATryingUE5GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
