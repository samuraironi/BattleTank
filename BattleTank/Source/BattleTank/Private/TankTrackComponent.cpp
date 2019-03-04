// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrackComponent.h"
#include "SprungWheel.h"
#include "Wheel.h"
#include "SpawnPoint.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/World.h"

UTankTrackComponent::UTankTrackComponent(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTankTrackComponent::TickComponent(float DeltaTime, ELevelTick tickType, FActorComponentTickFunction * thisTickFunction)
{
	Super::TickComponent(DeltaTime, tickType, thisTickFunction);

	LastLocation = GetComponentLocation().X;
	MoveTrack();
}


void UTankTrackComponent::Build(UInstancedStaticMeshComponent * mesh)
{
	if (mesh)
	{
		Mesh = mesh;
		Mesh->ClearInstances();

		const auto treadLength = GetSplineLength() / TreadCount;
		for (auto i = 0; i < TreadCount; i++)
		{
			Mesh->AddInstance(GetTransformAtDistanceAlongSpline(treadLength * i, ESplineCoordinateSpace::Local));
		}
	}

	for (auto wheel : GetWheels<ASprungWheel>())
	{
		wheel->WheelHitDelegate.BindUObject(this, &UTankTrackComponent::DriveTrack);
	}
}

void UTankTrackComponent::SetupSpline(int wheelId, FVector location, float wheelRadius)
{
	auto localWheelPos = GetComponentTransform().InverseTransformPosition(location);
	localWheelPos.Y = 0;

	FVector upVector = GetUpVectorAtSplinePoint(wheelId, ESplineCoordinateSpace::Local);
	FVector inLocation = localWheelPos - (wheelRadius + TrackTicknessOffset) * upVector;

	SetLocationAtSplinePoint(wheelId, inLocation, ESplineCoordinateSpace::Local);
}

void UTankTrackComponent::MoveTrack()
{
	const auto trackLength = GetSplineLength();
	const auto treadLength = GetSplineLength() / TreadCount;

	const auto lasttrackOffset = TrackOffsetPercentage * trackLength;
	auto trackOffset = FMath::Fmod(lasttrackOffset + DeltaX, trackLength);

	if (trackOffset < 0)
	{
		trackOffset = trackLength + trackOffset;
	}

	for (auto i = 0; i < TreadCount; i++)
	{
		const auto offset = FMath::Fmod(treadLength * i + trackOffset, trackLength);
		
		Mesh->UpdateInstanceTransform(i, GetTransformAtDistanceAlongSpline(offset, ESplineCoordinateSpace::Local), false, i == TreadCount - 1, false);
	}

	TrackOffsetPercentage = trackOffset / trackLength;
}

void UTankTrackComponent::SetThrottle(float throttle)
{
	CurrentThrottle = FMath::Clamp<float>(throttle, -1, 1);
	DeltaX = LastLocation - GetComponentLocation().X;
	//DriveTrack();
}

void UTankTrackComponent::DriveTrack()
{
	auto forceAplied = CurrentThrottle * TrackMaxDrivingForce;
	auto SpringWheels = GetWheels<ASprungWheel>();
	auto Wheels = GetWheels<AWheel>();

	auto forcePerWheel = forceAplied / (SpringWheels.Num());
	for (ASprungWheel* Wheel : SpringWheels)
	{
		Wheel->AddDrivingForce(forcePerWheel);
	}

	/*for (AWheel* Wheel : Wheels)
	{
		//Wheel->AddDrivingForce(ForcePerWheel);
	}*/
}
