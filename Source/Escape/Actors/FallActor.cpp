// Fill out your copyright notice in the Description page of Project Settings.


#include "FallActor.h"


AFallActor::AFallActor() 
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

void AFallActor::Act() 
{
    Mesh->SetMobility(EComponentMobility::Movable);
    Mesh->SetSimulatePhysics(true);
}

