// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"

#include "Gameframework/Actor.h"

// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();

	// -> operator is used to access the methods of a returned pointer. For example,
	// SomePointer->SomeMethod() is the same as (*SomePointer).SomeMethod()
	// Check if a method returns a pointer or an actual object by hovering over it 
	// with the cursor. If there is a * in the return, it is a pointer.

	// GetOwner() returns a pointer to the actor that this PostionReport component
	// belongs to. GetName() is a method for actors that returns the actor's name
	FString ObjectName = GetOwner()->GetName();

	// Access the actor transform via the pointer that GetOwner() returns. This method
	// returns an actual FTransform object, so the dot operator can be used. GetLocation()
	// returns an FVector which is just an x,y,z coordinate. Use ToString() to do
	// formatted printing.
	FString ObjectPos = GetOwner()->GetActorTransform().GetLocation().ToString();

	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos);
	
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

