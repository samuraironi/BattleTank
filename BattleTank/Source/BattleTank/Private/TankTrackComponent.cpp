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

void UTankTrackComponent::TickComponent(float DeltaTime, ELevelTick tickType, FActorComponentTickFunction * thisTickFunction)
{
	Super::TickComponent(DeltaTime, tickType, thisTickFunction);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0;
	}

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
}

void UTankTrackComponent::SetupSpline(int wheelId, FVector location, float wheelRadius)
{
	auto localWheelPos = GetComponentTransform().InverseTransformPosition(location);
	localWheelPos.Y = 0;

	int32 pointIndex = wheelId;

	FVector upVector = GetUpVectorAtSplinePoint(pointIndex, ESplineCoordinateSpace::Local);
	FVector inLocation = localWheelPos - (wheelRadius + TrackTicknessOffset) * upVector;

	SetLocationAtSplinePoint(pointIndex, inLocation, ESplineCoordinateSpace::Local);
}

void UTankTrackComponent::AddForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void UTankTrackComponent::MoveTrack()
{
	const auto trackLength = GetSplineLength();
	const auto treadLength = GetSplineLength() / TreadCount;

	const auto lasttrackOffset = TrackOffsetPercentage * trackLength;
	auto trackOffset = FMath::Fmod(lasttrackOffset - TotalForceMagnitudeThisFrame, trackLength);

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
	float CurrentThrottle = FMath::Clamp<float>(throttle, -1, 1);
	DriveTrack(CurrentThrottle);
}

void UTankTrackComponent::DriveTrack(float CurrentThrottle)
{
	auto forceAplied = CurrentThrottle * TrackMaxDrivingForce;
	auto SpringWheels = GetWheels<ASprungWheel>();
	auto Wheels = GetWheels<AWheel>();
	auto w = GetChildrenWheels<ASprungWheel>();

	auto ForcePerWheel = forceAplied / (SpringWheels.Num() + Wheels.Num());
	for (ASprungWheel* Wheel : SpringWheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}

	for (ASprungWheel* Wheel : w)
	{
		//Wheel->AddDrivingForce(ForcePerWheel);
	}

	for (AWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

template<class T>
TArray<T*> UTankTrackComponent::GetWheels() const
{
	TArray<T*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto Wheel = Cast<T>(SpawnedChild);
		if (!Wheel) continue;

		ResultArray.Add(Wheel);
	}
	return ResultArray;
}

template<class T>
TArray<T*> UTankTrackComponent::GetChildrenWheels() const
{
	TArray<T*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		auto SpawnPointChild = Cast<UChildActorComponent>(Child);
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetChildActor();
		auto Wheel = Cast<T>(SpawnedChild);
		if (!Wheel) continue;

		ResultArray.Add(Wheel);
	}
	return ResultArray;
}
