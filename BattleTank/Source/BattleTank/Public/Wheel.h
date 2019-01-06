// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wheel.generated.h"

class USphereComponent;

UCLASS()
class BATTLETANK_API AWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWheel();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float Forcemagnitude);

	void DriveWheel(float relativeSpeed);

	float GetRadius();

	FVector GetLocation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float TotalForceMagnitudeThisFrame = 0;

	void ApplyForce();

	float CurrentWheelPos = 0;

	int MaxDegreesPerSecond = 5;

	float Location = 0;

	float Radius = 0;

	UPROPERTY(EditAnywhere, Category = Setup)
	int Id = 0;

	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent* Wheel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* WheelMesh = nullptr;
	
};
