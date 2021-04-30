// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Escape/Characters/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void ABaseGameMode::BeginPlay() 
{
    Super::BeginPlay();

    ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));

    if (PlayerHUDClass)
    {
        PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
        if (PlayerHUD)
            PlayerHUD->AddToViewport();
    }
}

// void ABaseGameMode::ShowPaperWidget() 
// {
    
// }

// void ABaseGameMode::RemovePaperWidget() 
// {
    
// }
