// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "FallActor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AFallActor : public AInteractableActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

public:
	AFallActor();

	virtual void Act() override;
};
