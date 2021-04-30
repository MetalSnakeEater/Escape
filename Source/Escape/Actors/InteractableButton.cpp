// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableButton.h"
#include "InteractableDoor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Components/ArrowComponent.h"

AInteractableButton::AInteractableButton()
{
    ReadyState = true;

    DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
    DefaultRootComponent->SetupAttachment(RootComponent);    

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(DefaultRootComponent);

    DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
    DirectionArrow->SetupAttachment(DefaultRootComponent);  

}

void AInteractableButton::BeginPlay() 
{
    Super::BeginPlay();

    OutPosition = Mesh->GetRelativeTransform();
    InPosition.SetLocation(Mesh->GetRelativeTransform().GetLocation() - FVector(0.f, 10.f,0.f));
    InPosition.SetRotation(Mesh->GetRelativeTransform().GetRotation());
    InPosition.SetScale3D(Mesh->GetRelativeTransform().GetScale3D());

    ButtonDirection = 1.f;

    if (ButtonCurve)
    {
        FOnTimelineFloat TimelineCallback;
        FOnTimelineEventStatic TimelineFinishedCallback;

        TimelineCallback.BindUFunction(this, FName("ControlButton"));
        TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

        MyTimeline = NewObject<UTimelineComponent>(this, FName("ButtonAnimation"));
        MyTimeline->SetTimelineLength(.2f);
        MyTimeline->AddInterpFloat(ButtonCurve, TimelineCallback);
        MyTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);        
		MyTimeline->RegisterComponent();
    }
}

void AInteractableButton::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    if (MyTimeline != NULL)
	{
	    MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
    }
}

void AInteractableButton::Act() 
{
    if (DoorToOpen)
    {
        APawn* OurPawn = UGameplayStatics::GetPlayerPawn(this, 0);
        FVector PawnLocation = OurPawn->GetActorLocation();
        PlayerDirection = GetActorLocation() - PawnLocation;
        PlayerDirection = UKismetMathLibrary::LessLess_VectorRotator(PlayerDirection, GetActorRotation());

        if (ReadyState)
        {
            if(PlayerDirection.Z > 0.0f)
            {
                ButtonDirection = -1.f;
            }
            else
            {
                ButtonDirection = 1.f;
            }
            ReadyState = false;
            if (MyTimeline != NULL)
	        {
                MyTimeline->PlayFromStart();
            }
        }
        DoorToOpen->isLocked = false;
        DoorToOpen->Act();
        DoorToOpen->isLocked = true;
    }
}

void AInteractableButton::ControlButton() 
{
    float TimelineValue = MyTimeline->GetPlaybackPosition();
    float CurveFloatValue = ButtonCurve->GetFloatValue(TimelineValue);
    FVector NewLoc = FMath::Lerp(OutPosition.GetLocation(), InPosition.GetLocation(), CurveFloatValue);
    Mesh->SetRelativeLocation(NewLoc);
}


void AInteractableButton::SetState()
{
    MyTimeline->Reverse();
    ReadyState = true;
}
