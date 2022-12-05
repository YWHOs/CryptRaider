// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

#include "math/UnrealMathUtility.h"
// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	originalLocation = GetOwner() -> GetActorLocation();
}


// Called every frame
void UMover::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
	Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

	FVector targetLocation = originalLocation;
	if(isMove)
	{
		targetLocation = originalLocation + moveOffset;
	}
	FVector currentLocation = GetOwner() -> GetActorLocation();
	float speed = moveOffset.Length() / moveTime;

	FVector newLocation = FMath::VInterpConstantTo(currentLocation, targetLocation, _deltaTime, speed);
	GetOwner()->SetActorLocation(newLocation);
}

void UMover::SetShouldMove(bool _isMove)
{
	isMove = _isMove;
}