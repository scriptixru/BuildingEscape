// Copyright Ben Tristem 2016

#include "Grabber.h"
#include "Components/PrimitiveComponent.h"


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
	FindPhisicsHandleComponent();
	SetupInputComponent();

	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}

/// Look for attached Physics Handle
void UGrabber::FindPhisicsHandleComponent()
{
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		// Physics handle is not found
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}

}

/// Look for attached Input Component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// InputComponent is found
		//UE_LOG(LogTemp, Warning, TEXT("Input component is found"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		// InputComponent handle is not found
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}



void UGrabber::Grab()
{
	///UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhisycsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	///If we hit something  then attach a physics handle
	if (ActorHit)
	{
		// attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator(0.0f, 0.0f, 0.0f)
			//ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}


}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	PhysicsHandle->ReleaseComponent();
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{

		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

const FHitResult UGrabber::GetFirstPhisycsBodyInReach()
{

	/// Draw a red trace in the world to visual
	/*DrawDebugLine(
	GetWorld(),
	PlayerViewPointLocation,
	LineTraceEnd,
	FColor(255, 0, 0),
	false,
	0.f,
	0.f,
	10.f
	);*/

	/// Line-trace (AKA ray-cast out to reach distance)
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), true, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	/// See what we hit
	/*AActor* AActorHit = Hit.GetActor();
	if (AActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(AActorHit->GetName()))
	}*/
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// Log out to test 
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotation.ToString());

	return PlayerViewPointLocation;


}

FVector UGrabber::GetReachLineEnd()
{
	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// Log out to test 
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotation.ToString());

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;


}