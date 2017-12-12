// Copyright Ben Tristem 2016

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/TriggerVolume.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "OpenDoor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();


	//void OpenDoor();
	//void CloseDoor();

	UPROPERTY(BlueprintAssignable)
	FDoorEvent 	OnOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent 	OnClose;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//UPROPERTY(EditAnywhere)
	//float OpenAngle = -90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;


	//UPROPERTY(EditAnywhere)
	//float DoorCloseDelay = 1.f;

	//float LastDoorOpenTime;

	//UPROPERTY(EditAnywhere)
	//AActor* ActorThatOpens; //Remember pawn inherits from actor

	// The owning door
	AActor* Owner = nullptr; 

	// Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
