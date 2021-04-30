// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "Blueprint/UserWidget.h"


APlayerControllerBase::APlayerControllerBase() 
{
    PrimaryActorTick.bCanEverTick = true;
    
}

void APlayerControllerBase::BeginPlay() 
{
    Super::BeginPlay();
}

void APlayerControllerBase::ShowPaperWidget() 
{
    if (PaperWidgetClass)
    {
        PaperWidget = CreateWidget<UUserWidget>(GetWorld(), PaperWidgetClass);
        if (PaperWidget)
            PaperWidget->AddToViewport();
    }
}

void APlayerControllerBase::RemovePaperWidget() 
{
    if (PaperWidget)
        PaperWidget->RemoveFromParent();
}
