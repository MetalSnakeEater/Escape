// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableButton.h"
#include "ButtonRiddle.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AButtonRiddle : public AInteractableButton
{
	GENERATED_BODY()
public:
	// VARIABLES
	FString RightCombo[];

	virtual void Act() override;
};
