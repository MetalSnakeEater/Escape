// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlayerBase.h"
#include "Components/BoxComponent.h"
#include "InteractableDoor.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APressurePlayerBase::APressurePlayerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentMass = 0;
	MassToAct = 5.f;
	IsActivated = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);
	Box->OnComponentBeginOverlap.AddDynamic(this,&APressurePlayerBase::OnBeginOverlap);
	
}

// Called when the game starts or when spawned
void APressurePlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentEndOverlap.AddDynamic(this,&APressurePlayerBase::OnEndOverlap);
}

// Called every frame
void APressurePlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressurePlayerBase::OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor != this)
	{
	TArray<UPrimitiveComponent*> comps;
	OtherActor->GetComponents(comps);

	for (auto comp : comps)
	{
		if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(comp))
			CurrentMass += mesh->GetMass();
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentMass);
	if (!IsActivated && CurrentMass >= MassToAct)
		if (ActorToActivate)
		{
			if (ActivationSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ActivationSound, GetActorLocation());
				
			IsActivated = true;
			if (AInteractableDoor* Door = Cast<AInteractableDoor>(ActorToActivate))
			{
				Door->isLocked = false;
				Door->Act();
				Door->isLocked = true;
			}
			else
			{
				ActorToActivate->Act();
			}
		}
	}
}

void APressurePlayerBase::OnEndOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	TArray<UPrimitiveComponent*> comps;
	OtherActor->GetComponents(comps);
	
	for (auto comp : comps)
	{
		if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(comp))
			CurrentMass -= mesh->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentMass);
	if (IsActivated && CurrentMass < MassToAct)
		if (ActorToActivate)
		{
			IsActivated = false;
			if (AInteractableDoor* Door = Cast<AInteractableDoor>(ActorToActivate))
			{
				Door->isLocked = false;
				Door->Act();
				Door->isLocked = true;
			}
			else
			{
				ActorToActivate->Act();
			}
		}

}

