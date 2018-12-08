// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"


void UTankTrack::SetThrottle(float throttle)
{
	//auto time = GetWorld()->GetTimeSeconds();
	auto name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s Throttle %f"), *name, throttle);

	auto forceAplied = GetForwardVector() * throttle * TrackMaxDrivingForce;
	auto forceLocation = GetComponentLocation();
	auto tankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	tankRoot->AddForceAtLocation(forceAplied, forceLocation);
}

