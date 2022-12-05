// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* physicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if(physicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *physicsHandle -> GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Physics Handle"));
	}
}


// Called every frame
void UGrabber::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	if(physicsHandle && physicsHandle -> GetGrabbedComponent())
	{
		FVector targetLocation = GetComponentLocation() + GetForwardVector() * holdDistance;
		physicsHandle -> SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}

}
void UGrabber::Grab()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if(physicsHandle == nullptr)
	{
		return;
	}
	FHitResult hitResult;
	bool isHit = GetGrabbableInReach(hitResult);
	if(isHit)
	{
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent -> SetSimulatePhysics(true);
		hitComponent -> WakeAllRigidBodies();
		AActor* hitActor = hitResult.GetActor();
		hitActor -> Tags.Add("Grabbed");
		hitActor -> DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		physicsHandle -> GrabComponentAtLocationWithRotation(
			hitComponent, NAME_None, hitResult.ImpactPoint, GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	if(physicsHandle && physicsHandle -> GetGrabbedComponent())
	{
		AActor* grabbedActor = physicsHandle -> GetGrabbedComponent() -> GetOwner();
		grabbedActor -> Tags.Remove("Grabbed");
		physicsHandle -> ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* result = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if(result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a handlecomponent"));
	}
	return result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& _outHitResult) const
{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * maxGrabDistance;
	DrawDebugLine(GetWorld(), start, end, FColor::Red);
	DrawDebugSphere(GetWorld(), end, 10, 10, FColor::Blue, false, 0.5);

	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);
	return GetWorld() -> SweepSingleByChannel(
		_outHitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, sphere);
}

