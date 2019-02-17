// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"

class UPhysicsConstraintComponent;
class USphereComponent;

UCLASS()
class BATTLETANK_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprungWheel();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float Forcemagnitude);

	void DriveWheel(float relativeSpeed);

	float GetRadius();

	FVector GetLocation();

	int GetId();

	void SetId(int id);

	void BuildWheel();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float TotalForceMagnitudeThisFrame = 0;

	void SetupConstraints();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ApplyForce();

	float CurrentWheelPos = 0;

	int MaxDegreesPerSecond = 5;

	float Location = 0;

	float Radius = 0;

	int Id = 0;

	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent* Wheel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* WheelMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent* Axle = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UPhysicsConstraintComponent* MassWheelConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UPhysicsConstraintComponent* AxleWheelConstraint = nullptr;
};
