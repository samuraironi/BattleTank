// Fill out your copyright notice in the Description page of Project Settings.

#include "Wheel.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"


// Sets default values
AWheel::AWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	
	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("WheelMesh"));
	WheelMesh->SetupAttachment(Wheel);
	SetRootComponent(Wheel);
}

// Called when the game starts or when spawned
void AWheel::BeginPlay()
{
	Super::BeginPlay();
	
}

float AWheel::GetRadius()
{
	return  Wheel->GetScaledSphereRadius();
}

FVector AWheel::GetLocation()
{
	return Wheel->GetComponentLocation();
}

void AWheel::SetId(int id)
{
	Id = id;
}

// Called every frame
void AWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0;
	}
	//ApplyForce();
}

void AWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void AWheel::DriveWheel(float relativeSpeed)
{
	relativeSpeed = FMath::Clamp<float>(relativeSpeed, -1, +1);

	auto rotationChange = relativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;

	MaxDegreesPerSecond ++;

	if (CurrentWheelPos >= 360)
		CurrentWheelPos = 0;
	else
		CurrentWheelPos += rotationChange;

	Wheel->SetRelativeRotation(FRotator(CurrentWheelPos, 0, 0));
}

void AWheel::ApplyForce()
{
	//Wheel->AddForce(Wheel->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

