// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InteractableButton.generated.h"

class AInteractableDoor;
class UCurveFloat;
class UTimelineComponent;
class UArrowComponent;

UCLASS()
class ESCAPE_API AInteractableButton : public AInteractableActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultRootComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AInteractableButton();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Functions
	virtual void Act() override;

	UFUNCTION()
    void ControlButton();

	UFUNCTION()
    void SetState();

	// Variables
	UPROPERTY(EditInstanceOnly, category = ConnectedObject)
	AInteractableDoor* DoorToOpen;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ButtonCurve;

	UPROPERTY(EditAnywhere)
	UArrowComponent* DirectionArrow;

	bool ReadyState;
	FVector PlayerDirection;
	FTransform InPosition;
	FTransform OutPosition;
	float ButtonDirection;
	UTimelineComponent* MyTimeline;
};
