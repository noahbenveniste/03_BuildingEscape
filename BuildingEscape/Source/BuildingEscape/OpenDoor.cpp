// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "BuildingEscape.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"

// Tag for out parameters
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Get the owner of the component i.e. the door instance
	Owner = GetOwner();

	if (PressurePlate == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Collision volume for pressure plate missing"));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume every frame
	// If the total mass of the actors on the plate reach a certain threshold, open the door
	if (GetTotalMassOfActorsOnPlate() > PressurePlateMassTriggerThreshold)
	{
		OnOpen.Broadcast();
	}
	else
	{
		// Close the door
		OnClose.Broadcast();
	}

}

// Gets the total mass of actors on the plate
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	// Get the actors currently overlapping the trigger volume i.e. on the pressure plate (as an array)
	TArray<AActor*> ActorsOnPlate;

	if (PressurePlate == nullptr) { return 0.f; }

	PressurePlate->GetOverlappingActors(
		OUT ActorsOnPlate, // An out parameter for an array that will be populated with pointers to overlapping actors
		nullptr
	);
	
	// Iterate over the actor array, getting the mass of the individual actors and adding them to the sum
	for (AActor *CurrentActor : ActorsOnPlate)
	{
		double Mass = CurrentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		TotalMass += Mass;
		/*
		float *MassPointer = &Mass;
		UE_LOG(LogTemp, Warning, TEXT("%s weighs %f"), *CurrentActor->GetName(), *MassPointer);
	    */
	}
	// Return the total mass of all the overlapping actors
	return TotalMass;
}

