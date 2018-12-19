// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialise(UTankBarrel * barrel, UTankTurret * turret)
{
	Barrel = barrel;
	Turret = turret;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (RoundsLeft <= 0)
	{
		FiringState = EFiringStatue::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringStatue::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringStatue::Aiming;
	}
	else
	{
		FiringState = EFiringStatue::Locked;
	}

}


EFiringStatue UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();

	return !BarrelForward.Equals(AimDirection, 0.01);
}



void UTankAimingComponent::AimAt(FVector HitLocation)
{
	//if (!ensure(Barrel) && ProjectileBlueprint) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Calculate the OutLaunchVelocity
	bool bHaveAimSOlution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bHaveAimSOlution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel)) { return; }
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	//Always Yal the shortes way
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
	
	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringStatue::Locked || FiringState == EFiringStatue::Aiming)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
		auto socketRotation = Barrel->GetSocketRotation(FName("Projectile"));
		auto socketLocation = Barrel->GetSocketLocation(FName("Projectile"));

		auto projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			socketLocation,
			socketRotation
			);

		projectile->Launch(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}
