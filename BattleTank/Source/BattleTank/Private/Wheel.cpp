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

// Called every frame
void AWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0;
	}
}

void AWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void AWheel::ApplyForce()
{
	Wheel->AddForce(Wheel->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

