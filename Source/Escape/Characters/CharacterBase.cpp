// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Escape/Actors/InteractableActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#define MAX_GRAB_DISTANCE 250.f
#define MIN_GRAB_DISTANCE 100.f

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsComponent = CreateDefaultSubobject<UPhysicsHandleComponent>("Phsysics Component");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	ImpulseValue = 1000.f;
	GrabDistance = 250.f;	
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugLine(GetWorld(), Camera->GetComponentLocation(), Camera->GetComponentLocation() + Camera->GetForwardVector() * GrabDistance, FColor::Red, false, 2.f);
	if (PhysicsComponent->GetGrabbedComponent())
		PhysicsComponent->SetTargetLocation(Camera->GetComponentLocation() + Camera->GetForwardVector() * GrabDistance);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Binding moving axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	// Binding looking up and turning axis
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterBase::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterBase::AddControllerPitchInput);

	// Binding Grab Distance 
	PlayerInputComponent->BindAxis("ChangeGrabDistance", this, &ACharacterBase::ChangeGrabDistance);

	// Binding Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::Jump);

	// Binding Pick up action
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::Interact);

}

void ACharacterBase::MoveForward(float Val) 
{
	if (Val != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ACharacterBase::MoveRight(float Val) 
{
	if (Val != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void ACharacterBase::Interact() 
{
	if (PhysicsComponent->GetGrabbedComponent())
	{
		PhysicsComponent->ReleaseComponent();
		GrabDistance = MAX_GRAB_DISTANCE;
		return;
	}
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * GrabDistance;
	FHitResult HitResult;
	if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		if (AInteractableActor* InteractedActor = Cast<AInteractableActor>(HitResult.GetActor()))
		{
			InteractedActor->Act();
			return;
		}
		PickUp(HitResult);
	}
}

void ACharacterBase::PickUp(FHitResult HitResult) 
{
	if (PhysicsComponent->GetGrabbedComponent())
	{
		Drop();
		return;
	}
	if(HitResult.GetActor() && HitResult.GetActor()->IsRootComponentMovable())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is was tried to pick up"), *(HitResult.GetActor()->GetName()));
		PhysicsComponent->GrabComponentAtLocation(HitResult.GetComponent(), "", HitResult.GetComponent()->GetComponentLocation());
	}
}

void ACharacterBase::Drop() 
{
	PhysicsComponent->ReleaseComponent();
	GrabDistance = MAX_GRAB_DISTANCE;
}

void ACharacterBase::Throw() 
{	
	if(PhysicsComponent->GetGrabbedComponent())
	{
		auto Temp = PhysicsComponent->GetGrabbedComponent();
		Drop();
		Temp->AddImpulse(Camera->GetForwardVector() * ImpulseValue);
	}
}

void ACharacterBase::ChangeGrabDistance(float Val) 
{
	float ChangedGrabDistance = GrabDistance + 50 * Val;
	if (PhysicsComponent->GetGrabbedComponent() && ChangedGrabDistance >= MIN_GRAB_DISTANCE && ChangedGrabDistance <= MAX_GRAB_DISTANCE)
		GrabDistance = ChangedGrabDistance;
}

