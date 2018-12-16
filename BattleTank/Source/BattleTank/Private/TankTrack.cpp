// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	//cos between vector of the speed And component speed(velocity)
	auto slippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	//calculate acceleration of stopping slipping (reason why - slippageSpeed)
	auto correctionAcseleration = -slippageSpeed / DeltaTime * GetRightVector();

	auto tank = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	//F = ma divided by 2 - because there're 2 tracks
	auto correctionForce = (tank->GetMass() * correctionAcseleration) / 2;
	tank->AddForce(correctionForce);
}

void UTankTrack::SetThrottle(float throttle)
{
	auto forceAplied = GetForwardVector() * throttle * TrackMaxDrivingForce;
	auto forceLocation = GetComponentLocation();
	auto tankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	tankRoot->AddForceAtLocation(forceAplied, forceLocation);
}