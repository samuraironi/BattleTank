// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "Wheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTankTrack::SetThrottle(float throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(throttle, -1, 1);
	DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto forceAplied = CurrentThrottle * TrackMaxDrivingForce;
	auto SpringWheels = GetWheels<ASprungWheel>();
	auto Wheels = GetWheels<AWheel>();
	auto ForcePerWheel = forceAplied / (SpringWheels.Num() + Wheels.Num());
	for (ASprungWheel* Wheel : SpringWheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}

	for (AWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

template<class T>
TArray<T*> UTankTrack::GetWheels() const
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
