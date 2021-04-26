// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"

AInteractableDoor::AInteractableDoor() 
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Open = false;
    ReadyState = true;

    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    RootComponent = DoorFrame;

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    Door->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

    RotateValue = 90.0f;

    if (DoorCurve)
    {
        FOnTimelineFloat TimelineCallback;
        FOnTimelineEventStatic TimelineFinishedCallback;

        TimelineCallback.BindUFunction(this, FName("ControlDoor"));
        TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

        MyTimeline = NewObject<UTimelineComponent>(this, FName("DoorAnimation"));
        MyTimeline->AddInterpFloat(DoorCurve, TimelineCallback);
        MyTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);        
		MyTimeline->RegisterComponent();
    }
	
}

// Called every frame
void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (MyTimeline != NULL)
	{
	    MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
    }

}

void AInteractableDoor::Act() 
{
	ToggleDoor();
}

void AInteractableDoor::ControlDoor()
{
    TimelineValue = MyTimeline->GetPlaybackPosition();
    CurveFloatValue = RotateValue*DoorCurve->GetFloatValue(TimelineValue);

    FQuat NewRotation = FQuat(FRotator(0.f, CurveFloatValue, 0.f));

    Door->SetRelativeRotation(NewRotation);
}

void AInteractableDoor::SetState()
{
    ReadyState = true;
}

void AInteractableDoor::ToggleDoor() 
{
    if(ReadyState) 
    {
        Open = !Open;

        // alternative way to get pawn position
        // GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()
        APawn* OurPawn = UGameplayStatics::GetPlayerPawn(this, 0);
        FVector PawnLocation = OurPawn->GetActorLocation();
        FVector Direction = GetActorLocation() - PawnLocation;
        Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

        if(Open)
        {     
            if(Direction.X > 0.0f)
            {
                RotateValue = 90.0f;
            }
            else
            {
                RotateValue = -90.0f;
            }

            ReadyState = false;
            if (MyTimeline != NULL)
	        {
                MyTimeline->PlayFromStart();
            }
        }
        else 
        {
            ReadyState = false;
            if (MyTimeline != NULL)
	        {
                MyTimeline->Reverse();
            }
        }
    }
}