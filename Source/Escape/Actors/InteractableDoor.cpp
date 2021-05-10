// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "Escape/Actor Components/KeyComponent.h"
#include "Sound/SoundBase.h"

AInteractableDoor::AInteractableDoor() 
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Open = false;
    ReadyState = true;
    isLocked = false;

    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    RootComponent = DoorFrame;

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    Door->SetupAttachment(RootComponent);

    KeyTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyTriggerBox"));
    KeyTriggerBox->SetupAttachment(RootComponent);  

    //KeyTriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AInteractableDoor::SetLockeState);
}

// Called when the game starts or when spawned
void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
    KeyTriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AInteractableDoor::SetLockeState);

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
    if (!isLocked)
	    ToggleDoor();
    else
    {
        Response("Locked");
        FTimerDelegate TimerDel;
        FTimerHandle TimerHandle;
        TimerDel.BindUFunction(this, FName("Response"));
        GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, .5f, false);
    } 
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
                if (OpenSound)
                {
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation(), 1.f, 1.f, 0.3f);
                }
                MyTimeline->PlayFromStart();
            }
        }
        else 
        {
            ReadyState = false;
            if (MyTimeline != NULL)
	        {
                if (OpenSound)
                {
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation(), 1.f, 1.f, 0.3f);
                }
                MyTimeline->Reverse();
            }
        }
    }
}

void AInteractableDoor::SetLockeState(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    if (isLocked)
    {
        TArray<UKeyComponent*> comps;
        OtherActor->GetComponents<UKeyComponent>(comps);

        for (auto comp : comps)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s"), *(comp->GetName()));
            if (UKeyComponent* key = Cast<UKeyComponent>(comp))
            {
                if (UnlockSound)
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), UnlockSound, GetActorLocation(), 5.f);
                isLocked = false;
                OtherActor->Destroy();
                return;
            }
        }
    }
}