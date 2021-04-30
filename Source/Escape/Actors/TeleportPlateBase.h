// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportPlateBase.generated.h"

class UBoxComponent;

UCLASS()
class ESCAPE_API ATeleportPlateBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;
	
public:	
	// Sets default values for this actor's properties
	ATeleportPlateBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly)
	ATeleportPlateBase* TeleportPlate;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* TeleportPoint;

	UFUNCTION()
	void Teleport(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
