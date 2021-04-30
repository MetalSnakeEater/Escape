// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPlateBase.h"
#include "Components/BoxComponent.h"

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
		PlayerPawn->GetController()->SetControlRotation(TeleportPlate->GetActorRotation());
		PlayerPawn->SetActorLocation(TeleportPlate->TeleportPoint->GetComponentLocation());
	}
}

