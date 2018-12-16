// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;
}


void UTankTrack::ApplySidewaysForce()
{
	//cos between vector of the speed And component speed(velocity)
	auto slippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	
	//calculate acceleration of stopping slipping (reason why - slippageSpeed)
	auto deltaTime = GetWorld()->GetDeltaSeconds();
	auto correctionAcseleration = -slippageSpeed / deltaTime * GetRightVector();

	auto tank = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	//F = ma divided by 2 - because there're 2 tracks
	auto correctionForce = (tank->GetMass() * correctionAcseleration) / 2;
	tank->AddForce(correctionForce);
}

void UTankTrack::SetThrottle(float throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + throttle, -1, 1);
}

void UTankTrack::DriveTrack()
{
	auto forceAplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto forceLocation = GetComponentLocation();
	auto tankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	tankRoot->AddForceAtLocation(forceAplied, forceLocation);
}