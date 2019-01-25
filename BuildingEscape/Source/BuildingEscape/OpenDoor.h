// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "OpenDoor.generated.h"

// Define new class FDoorEvent; used for broadcasting events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// This macro is also shown in the same place as OpenAngle but is also editable
	// A trigger volume is a volume of space in a game world that can trigger events
	UPROPERTY(EditAnywhere)
		ATriggerVolume *PressurePlate = nullptr;

	// The amount of mass that must be on the pressure plate at one time to get it to open the door
	UPROPERTY(EditAnywhere)
		float PressurePlateMassTriggerThreshold = 50.f;

	float GetTotalMassOfActorsOnPlate();

	AActor *Owner = nullptr; // Door instance that this component belongs to
};
