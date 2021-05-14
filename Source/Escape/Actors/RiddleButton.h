// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "RiddleButton.generated.h"

class AInteractableButton;
class USoundBase;

UCLASS()
class ESCAPE_API ARiddleButton : public AInteractableActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditInstanceOnly)
	USceneComponent* SoundPlayLocation;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
public:
	ARiddleButton();

	virtual void Act(AInteractableButton* button);

	UPROPERTY(EditInstanceOnly)
	AInteractableActor* ActorsArr[4];

	UPROPERTY(EditInstanceOnly)
	AInteractableActor* ToTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* WrongSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* RightSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* SlidingWall;

	int CurrentInd = 0;
};
