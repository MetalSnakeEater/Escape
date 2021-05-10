// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InteractablePaper.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AInteractablePaper : public AInteractableActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
public:
	AInteractablePaper();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Text)
	FString Text;
	UPROPERTY(BlueprintReadOnly)
	bool isRead;

	virtual void Act() override;
};
