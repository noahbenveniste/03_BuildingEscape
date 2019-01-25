// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

// A custom macro that does nothing, used to anotate out parameters i.e. objects that are being changed by having
// their references passed to a method call
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();

	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	/// Look for input component (auto generated at run time for default pawn)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		/// Bind the input action. When the key associated with the Grab action is pressed, look for the Grab() method associated with this instance
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		/// If the grab key is released, call the Release() method
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		/// Input component not found, throw an error
		UE_LOG(LogTemp, Error, TEXT("%s missing InputComponent component"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached physics handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		/// If a physics handle component is not added to the object that has the grabber, throw an error
		UE_LOG(LogTemp, Error, TEXT("%s missing PhysicsHandle component"), *GetOwner()->GetName());
	}
}

// Ray-cast out, grab what's in reach
void UGrabber::Grab()
{
	// Line trace and try to reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	// What part of the thing we're grabbing should we grab
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();

	// The thing we hit with the line trace
	AActor *ActorHit = HitResult.GetActor();
	
	// Check for a nullptr with the PhysicsHandle
	if (PhysicsHandle == nullptr) { return; }

	// If we hit anything, attach a physics handle to it
	if (ActorHit) //NOTE this notation is the same as if (ActorHit != nullptr)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,									// The thing we're grabbing
			NAME_None,											// Name of the bone; none since we're using static meshes and not skeletons
			ComponentToGrab->GetOwner()->GetActorLocation(),	// Where we grab the thing
			true												// Allow rotation
		);
	}
}

// Release the currently grabbed object
void UGrabber::Release()
{
	// Check for a nullptr with the PhysicsHandle
	if (PhysicsHandle == nullptr) { return; }

	// Release physics handle
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///* Ray-cast (aka line trace) out to reach distance *///

	// Out parameter for the hit result; a STRUCT
	FHitResult HitResult;

	// A struct that stores the types of objects we are interested in querying, based on an input collision channel.
	// The parameter for this constructor is of type ENUM; we indicate that we only want to collide with PHYSICS BODIES.
	FCollisionObjectQueryParams ObjectsToQuery = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);

	/// Set up query parameters

	// Defines parameters for collision function.
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(
		FName(TEXT("")),	// Leave this empty for now
		false,				// Want to use SIMPLE trace type i.e. uses a simple hitbox instead of complex
		GetOwner()			// Want to ignore ourselves, otherwise the first thing the trace would intersect would be ourselves
	);

	// Whether or not the trace intersected any filtered objects this frame
	bool bIntersectPhysicsObject = false;

	bIntersectPhysicsObject = GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,				 // The first hit that is found that blocks the ray
		GetReachLineStart(), // Start location of the query ray
		GetReachLineEnd(),			 // End location of the query ray
		ObjectsToQuery,			 // The object types we want to query
		TraceParameters			 // Parameters for the collision function
	);

	/// Log any hits this frame
	if (bIntersectPhysicsObject)
	{
		// The physics object actor that was hit this frame
		AActor *ActorThatWasHit = HitResult.GetActor();
		// The name of the actor that was hit
		FString ActorName = *ActorThatWasHit->GetName();

		/// Log to console
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorName);
	}
	return HitResult;
}

// Gets the start of the line that defines the grabber's reach
FVector UGrabber::GetReachLineStart()
{
	// Stores the player location data returned by GetPlayerViewPoint()
	FVector PlayerViewPointLocation;
	// Stores the player rotation data returned by GetPlayerViewPoint()
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

// Gets the end of the line that defines the grabber's reach
FVector UGrabber::GetReachLineEnd()
{
	/// Get the player viewpoint
	/// GetPlayerViewPoint() directly edits the objects associated with the references passed to it

	// Stores the player location data returned by GetPlayerViewPoint()
	FVector PlayerViewPointLocation;
	// Stores the player rotation data returned by GetPlayerViewPoint()
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// A unit vector that follows the rotation of the player pawn.
	FVector LineTraceDirection = PlayerViewPointRotation.Vector();
	// This is the vector sum of the vector going from the origin to the player and the vector going from the player
	// (in the direction of their rotation) out to the reach. This point will be the end of the debug line.
	return PlayerViewPointLocation + LineTraceDirection * Reach;
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check for a nullptr with the PhysicsHandle
	if (PhysicsHandle == nullptr) { return; } // Breaks out of this iteration of the hot loop

	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object that we're holding each frame to the end of our grabber line trace
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

