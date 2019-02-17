// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"
#include "Wheel.h"
#include "SprungWheel.h"
#include "TankTrackComponent.h"

void UTankMovementComponent::Initialise(UTankTrackComponent* leftTrackComponent, UTankTrackComponent* rightTrackComponent)
{
	LeftTrack = leftTrackComponent;
	RightTrack = rightTrackComponent;
}

void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	//UE_LOG(LogTemp, Warning, TEXT("Tick from movement"));
	if (LeftTrack)
	{
		auto leftWheels = LeftTrack->GetWheels<AWheel>();
		auto leftSpringWheels = LeftTrack->GetWheels<ASprungWheel>();
		for (int i = 0; i < leftSpringWheels.Num(); i++)
		{
			auto wheel = leftSpringWheels[i];
			LeftTrack->SetupSpline(wheel->GetId(), wheel->GetLocation(), wheel->GetRadius());
		}
		for (int i = 0; i < leftWheels.Num(); i++)
		{

		}

	}
	if (RightTrack)
	{
		auto rightWheels = RightTrack->GetWheels<AWheel>();
		auto rightSpringWheels = RightTrack->GetWheels<ASprungWheel>();
		for (int i = 0; i < rightSpringWheels.Num(); i++)
		{
			auto wheel = rightSpringWheels[i];
			RightTrack->SetupSpline(wheel->GetId(), wheel->GetLocation(), wheel->GetRadius());
		}
		for (int i = 0; i < rightWheels.Num(); i++)
		{

		}
	}
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	auto TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	auto recuiredThrow = FVector::DotProduct(TankForwardDirection, AIForwardIntention);
	IntendMoveForward(recuiredThrow);

	auto turnRotateThrown = FVector::CrossProduct(TankForwardDirection, AIForwardIntention).Z;
	IntendRotate(turnRotateThrown);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendRotate(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
	
}