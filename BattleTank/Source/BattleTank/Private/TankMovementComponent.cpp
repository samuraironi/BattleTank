// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack * leftTrack, UTankTrack * rightTrack)
{
	if (!leftTrack || !rightTrack) { return; }
	LeftTrack = leftTrack;
	RightTrack = rightTrack;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	//auto time = GetWorld()->GetTimeSeconds();
	auto name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}


