// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InterActor_LvlSequenceTrigger.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;

UCLASS()
class ESCAPE_API AInterActor_LvlSequenceTrigger : public AInteractableActor
{
	GENERATED_BODY()

public:
	virtual void Act() override;

	// Variables
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	ULevelSequence* LevelSequence;

	UPROPERTY()
	ULevelSequencePlayer* LevelSequencePlayer;

	bool isPlayed = false;
};
