// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Find (assumed) attached physics handle component
	void FindPhysicsHandleComponent();

	// Gets the start of the grabber reach
	FVector GetReachLineStart();

	// Gets the end of the grabber reach
	FVector GetReachLineEnd();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Ray-cast and grab what's in reach
	void Grab();

	// Release the grabbed object when the grab key is released
	void Release();

	// Return hit for first physics body reached
	const FHitResult GetFirstPhysicsBodyInReach();

private:
	// How far the player can reach ahead of them
	float Reach = 100.f;
	
	UPhysicsHandleComponent *PhysicsHandle = nullptr;

	UInputComponent *InputComponent = nullptr;
};
