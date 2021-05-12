// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Escape/Actors/InteractableActor.h"
#include "Escape/Actors/InteractablePaper.h"
#include "Escape/Actor Components/FallActorComponent.h"
#include "Escape/GameModes/BaseGameMode.h"
#include "Escape/PlayerControllers/PlayerControllerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundConcurrency.h"
#define MAX_GRAB_DISTANCE 250.f
#define MIN_GRAB_DISTANCE 100.f

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed *= .5f;

	PhysicsComponent = CreateDefaultSubobject<UPhysicsHandleComponent>("Phsysics Component");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	ImpulseValue = 1000.f;
	GrabDistance = 250.f;
	Opacity = 0.5f;
	ActorResponse = "";
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	RunSpeed = 1.5f;
	Stamina = 100.f;
	isReading = false;
	isRunning = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	RespawnPoint = GetActorLocation();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isLookAtInterctable();

	if (PhysicsComponent->GetGrabbedComponent())
	{
		//UE_LOG(LogTemp,Warning, TEXT("%f"), (GetActorLocation() - PhysicsComponent->GrabbedComponent->GetComponentLocation()).GetAbs().Size());
		if ((GetActorLocation() - PhysicsComponent->GrabbedComponent->GetComponentLocation()).GetAbs().Size() > MAX_GRAB_DISTANCE)
		{
			PhysicsComponent->GetGrabbedComponent()->SetWorldLocation(Camera->GetComponentLocation() + Camera->GetForwardVector() * GrabDistance);
		}
		else
			PhysicsComponent->SetTargetLocation(Camera->GetComponentLocation() + Camera->GetForwardVector() * GrabDistance);
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Binding moving axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	// Binding looking up and turning axis
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterBase::LookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterBase::LookUp);

	// Binding Grab Distance 
	PlayerInputComponent->BindAxis("ChangeGrabDistance", this, &ACharacterBase::ChangeGrabDistance);

	// Binding Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::Jump);

	// Binding Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharacterBase::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACharacterBase::Uncrouch);

	// Binding Run
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACharacterBase::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACharacterBase::StopRun);

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

void ACharacterBase::LookUp(float Val) 
{	
	if (!isReading)
		AddControllerPitchInput(Val);
}

void ACharacterBase::LookRight(float Val) 
{
	if (!isReading)
		AddControllerYawInput(Val);
}

void ACharacterBase::Crouch() 
{
	UE_LOG(LogTemp, Warning, TEXT("Crouch"));
	//GetCharacterMovement()->Crouch();
	GetCharacterMovement()->bWantsToCrouch = true;
}

void ACharacterBase::Uncrouch() 
{
	UE_LOG(LogTemp, Warning, TEXT("Uncrouch"));
	//GetCharacterMovement()->UnCrouch();
	GetCharacterMovement()->bWantsToCrouch = false;
}

void ACharacterBase::DecreaseStamina() 
{
	if (Stamina > 0.f)
		Stamina -= .1f;
	else
		StopRun();
}

void ACharacterBase::RestoreStamina() 
{
	if (Stamina < 100)
		Stamina += .1f;
	else
		GetWorldTimerManager().ClearTimer(TimeHandleStopRun);
}

void ACharacterBase::Run() 
{
	if (!GetCharacterMovement()->IsCrouching())
	{
		isRunning = true;
		GetCharacterMovement()->MaxWalkSpeed *= RunSpeed;
		GetWorldTimerManager().ClearTimer(TimeHandleStopRun);
		GetWorldTimerManager().SetTimer(TimeHandleRun, this, &ACharacterBase::DecreaseStamina, .01f, true);
	}
}

void ACharacterBase::StopRun() 
{
	isRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetWorldTimerManager().ClearTimer(TimeHandleRun);
	GetWorldTimerManager().SetTimer(TimeHandleStopRun, this, &ACharacterBase::RestoreStamina, .01f, true);
}


void ACharacterBase::Interact() 
{
	if (isReading)
	{
		isReading = false;
		if (PaperSoundDown)
				UGameplayStatics::PlaySound2D(GetWorld(), PaperSoundDown, 10.f, 1.f, 0.6f);

		APlayerControllerBase* PlayerCon = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerCon)
				PlayerCon->RemovePaperWidget();

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		return;
	}
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
		if (AInteractablePaper* Paper = Cast<AInteractablePaper>(HitResult.GetActor()))
		{
			isReading = true;
			if (PaperSound)
				UGameplayStatics::PlaySound2D(GetWorld(), PaperSound, 1.f, 1.f, 0.2f);

			PaperText = Paper->Text;
			APlayerControllerBase* PlayerCon = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PlayerCon)
				PlayerCon->ShowPaperWidget();
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			return;
		}
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
		GrabDistance = MIN_GRAB_DISTANCE;
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

AInteractableActor* ACharacterBase::isLookAtInterctable() 
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * GrabDistance;
	FHitResult HitResult;
	if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		AInteractableActor* InteractedActor = Cast<AInteractableActor>(HitResult.GetActor());
		if (HitResult.GetActor() && (InteractedActor || HitResult.GetActor()->IsRootComponentMovable()))
		{
			Opacity = 1.f;
			return InteractedActor;
		}
	}
	Opacity = 0.5f;
	return nullptr;
}

void ACharacterBase::SetRespawnPoint(FVector Loc) 
{
	RespawnPoint = Loc;
}

void ACharacterBase::Respawn() 
{
	GetCharacterMovement()->StopMovementImmediately();
	SetActorLocation(RespawnPoint);
}
