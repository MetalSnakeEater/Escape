// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPlateBase.h"
#include "Components/BoxComponent.h"
#include "Escape/Characters/CharacterBase.h"

// Sets default values
ATeleportPlateBase::ATeleportPlateBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

	TeleportPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Teleport Point"));
    TeleportPoint->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);
	Box->OnComponentBeginOverlap.AddDynamic(this,&ATeleportPlateBase::Teleport);

}

// Called when the game starts or when spawned
void ATeleportPlateBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportPlateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeleportPlateBase::Teleport(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (TeleportPlate && PlayerPawn == Pawn)
	{
		PlayerPawn->DisableInput(GetWorld()->GetFirstPlayerController());
		ACharacterBase* Character = Cast<ACharacterBase>(PlayerPawn);
		if (Character)
			Character->StopRun();
		FTimerHandle TimerHandle;
		FTimerHandle TeleportTimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATeleportPlateBase::EnableMoving, 2.f, false);
		GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ATeleportPlateBase::TeleportPlayer, 2.f, false);
	}
}

void ATeleportPlateBase::EnableMoving() 
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn)
	{
		PlayerPawn->EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

void ATeleportPlateBase::TeleportPlayer() 
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn)
	{
		PlayerPawn->GetController()->SetControlRotation(TeleportPlate->GetActorRotation());
		PlayerPawn->SetActorLocation(TeleportPlate->TeleportPoint->GetComponentLocation() + FVector(0.f, 0.f, 10.f));
	}
}

