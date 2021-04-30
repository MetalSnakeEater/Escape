// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Escape/Characters/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::Act() 
{
	
}

void AInteractableActor::Response(FString response) 
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player)
		Player->ActorResponse = response;
}

