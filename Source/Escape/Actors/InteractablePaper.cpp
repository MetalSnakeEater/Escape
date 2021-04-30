// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractablePaper.h"

AInteractablePaper::AInteractablePaper() 
{
    isRead = false;
    Text = "";

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetupAttachment(RootComponent);
}

void AInteractablePaper::Act() 
{
    isRead = true;
}
