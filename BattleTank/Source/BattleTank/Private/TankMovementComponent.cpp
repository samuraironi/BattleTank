// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"
#include "Wheel.h"
#include "SprungWheel.h"
#include "TankTrackComponent.h"

void UTankMovementComponent::Initialise(UTankTrackComponent* leftTrackComponent, UTankTrackComponent* rightTrackComponent)
{
	//LeftTrack = leftTrack;
	//RightTrack = rightTrack;

	LeftTrackComponent = leftTrackComponent;
	RightTrackComponent = rightTrackComponent;
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	UE_LOG(LogTemp, Warning, TEXT("request direct move"));
	auto TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	auto recuiredThrow = FVector::DotProduct(TankForwardDirection, AIForwardIntention);
	IntendMoveForward(recuiredThrow);

	auto turnRotateThrown = FVector::CrossProduct(TankForwardDirection, AIForwardIntention).Z;
	IntendRotate(turnRotateThrown);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	//if (!ensure(LeftTrack && RightTrack)) { return; }
	//LeftTrack->SetThrottle(Throw);
	//RightTrack->SetThrottle(Throw);

	if (!ensure(LeftTrackComponent && RightTrackComponent)) { return; }
	LeftTrackComponent->SetThrottle(Throw);
	RightTrackComponent->SetThrottle(Throw);

	/*auto leftWheels = LeftTrack->GetWheels<AWheel>();
	auto leftSpringWheels = LeftTrack->GetWheels<ASprungWheel>();
	for (AWheel* wheel : leftWheels)
	{
		auto forceAplied = Throw * LeftTrack->TrackMaxDrivingForce;
		auto ForcePerWheel = forceAplied / (leftSpringWheels.Num() + leftWheels.Num());
		wheel->DriveWheel(-ForcePerWheel);
	}
	for (ASprungWheel* wheel : leftSpringWheels)
	{
		auto forceAplied = Throw * LeftTrack->TrackMaxDrivingForce;
		auto ForcePerWheel = forceAplied / (leftSpringWheels.Num() + leftWheels.Num());
		wheel->DriveWheel(-ForcePerWheel);
	}

	auto rightWheels = RightTrack->GetWheels<AWheel>();
	auto rightSpringWheels = RightTrack->GetWheels<ASprungWheel>();
	for (AWheel* wheel : rightWheels)
	{
		auto forceAplied = Throw * RightTrack->TrackMaxDrivingForce;
		auto ForcePerWheel = forceAplied / (rightSpringWheels.Num() + rightWheels.Num());
		wheel->DriveWheel(-ForcePerWheel);
	}
	for (ASprungWheel* wheel : rightSpringWheels)
	{
		auto forceAplied = Throw * RightTrack->TrackMaxDrivingForce;
		auto ForcePerWheel = forceAplied / (rightSpringWheels.Num() + rightWheels.Num());
		wheel->DriveWheel(-ForcePerWheel);
	}*/
}

void UTankMovementComponent::IntendRotate(float Throw)
{
	//if (!ensure(LeftTrack && RightTrack)) { return; }
	//LeftTrack->SetThrottle(Throw);
	//RightTrack->SetThrottle(-Throw);
}