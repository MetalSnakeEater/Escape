// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPhysicsHandleComponent;
class UWidgetComponent;
class AInteractableActor;
class AInteractablePaper;
class UFallActorComponent;

UCLASS()
class ESCAPE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = PhysicsHandle)
	UPhysicsHandleComponent* PhysicsComponent;


public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// VARIABLES
	UPROPERTY(EditDefaultsOnly, Category = Impulse)
	float ImpulseValue;

	UPROPERTY(EditDefaultsOnly, Category = Impulse)
	float GrabDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Opacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float Stamina;

	UPROPERTY(BlueprintReadOnly)
	bool isReading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString ActorResponse;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString PaperText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<UFallActorComponent> FallComp;

	float DefaultWalkSpeed;


	FTimerHandle TimeHandleRun;
	FTimerHandle TimeHandleStopRun;

	// FUNCTIONS
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Moving functions
	void MoveForward(float Val);
	void MoveRight(float Val);
	void Run();
	void StopRun();

	// Looking
	void LookUp(float Val);
	void LookRight(float Val);

	// Action functions
	void Interact();
	void PickUp(FHitResult HitResult);
	void Drop();
	void Throw();
	void ChangeGrabDistance(float Val);
	void DecreaseStamina();
	void RestoreStamina();
	void Crouch();
	void Uncrouch();

	UFUNCTION(BlueprintCallable)
	AInteractableActor* isLookAtInterctable();

};
