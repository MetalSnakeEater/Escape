// Fill out your copyright notice in the Description page of Project Settings.


#include "RiddleButton.h"
#include "InteractableButton.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

ARiddleButton::ARiddleButton() 
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    SoundPlayLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Sound Play Location"));
}

void ARiddleButton::Act(AInteractableButton* button) 
{
    UE_LOG(LogTemp, Warning, TEXT("%i"),CurrentInd);

    if (ActorsArr[CurrentInd] == button)
        CurrentInd++;
    else
    {
        CurrentInd = 0;
        UGameplayStatics::PlaySound2D(GetWorld(), WrongSound);
    }

    if (CurrentInd == 4)
        if (ToTrigger)
        {
            if (RightSound)
                UGameplayStatics::PlaySound2D(GetWorld(), RightSound);
            if (SlidingWall)
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlidingWall, SoundPlayLocation->GetComponentLocation(), 1.f, 1.f, 4.f);
            ToTrigger->Act();
            return;
        }
}

