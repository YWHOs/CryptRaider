// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* _mover);
private:
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	UMover* mover;

	AActor* GetAcceptableActor() const;

};
