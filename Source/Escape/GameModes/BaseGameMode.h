// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

class UUserWidget;

UCLASS()
class ESCAPE_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, category = HUD)
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY(EditDefaultsOnly, category = HUD)
	UUserWidget* PlayerHUD;

	// UPROPERTY(EditDefaultsOnly, category = HUD)
	// TSubclassOf<UUserWidget> PaperWidgetClass;

	// UPROPERTY(EditDefaultsOnly, category = HUD)
	// UUserWidget* PaperWidget;

	// void ShowPaperWidget();
	// void RemovePaperWidget();
};
