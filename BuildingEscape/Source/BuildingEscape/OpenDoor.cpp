// Copyright Ben Tristem 2016

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

#define OUT




// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	// Find the owning Actor
	Owner = GetOwner();
	if (!PressurePlate)
	{
		// InputComponent handle is not found
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	
}

/*void UOpenDoor::OpenDoor()
{

	// Set the door rotation
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}*/

/*void UOpenDoor::CloseDoor()
{

	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}*/


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the Trigger Volume 
	//If the ActorThatOpen is in the volume
	//if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	if(GetTotalMassOfActorsOnPlate() > TriggerMass) 
	{
		OnOpen.Broadcast();
		//OpenDoor();
		//LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	// Check if it's time to close the door
	//if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		OnClose.Broadcast();
		//CloseDoor();
	}
	
}
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	
	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}


