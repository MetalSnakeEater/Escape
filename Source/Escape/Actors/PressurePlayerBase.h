// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.h"
#include "PressurePlayerBase.generated.h"

class UBoxComponent;

UCLASS()
class ESCAPE_API APressurePlayerBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;
public:	
	// Sets default values for this actor's properties
	APressurePlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Variables
	UPROPERTY(EditAnywhere)
	AInteractableActor* ActorToActivate;

	UPROPERTY(EditAnywhere)
	float MassToAct;
	
	float CurrentMass;
	bool IsActivated;
};
