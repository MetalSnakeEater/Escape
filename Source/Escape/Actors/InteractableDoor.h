// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InteractableDoor.generated.h"

class UCurveFloat;
class UTimelineComponent;
class UBoxComponent;
class USoundBase;

UCLASS()
class ESCAPE_API AInteractableDoor : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Door;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorFrame;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DoorCurve;

	UPROPERTY(EditAnywhere)
	UBoxComponent* KeyTriggerBox;

	// Functions
	virtual void Act() override;

	UFUNCTION()
    void ControlDoor();

	UFUNCTION()
    void ToggleDoor();

	UFUNCTION()
    void SetState();

	UFUNCTION()
    void SetLockeState(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isLocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* OpenSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* UnlockSound;

	bool Open;
	bool ReadyState;
	float RotateValue;
	float CurveFloatValue;
	float TimelineValue;
	UTimelineComponent* MyTimeline;
};
