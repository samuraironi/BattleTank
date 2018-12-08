// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Engine/World.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//create components into blueprints from code
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Components"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::AimAt(FVector HitLocation)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::SetBarrelReference(UTankBarrel * barrel)
{
	if (!barrel) { return; }
	TankAimingComponent->SetBarrelReference(barrel);
	Barrel = barrel;
}

void ATank::SetTurretReference(UTankTurret * turret)
{
	if (!turret) { return; }
	TankAimingComponent->SetTurretReference(turret);
}

void ATank::Fire()
{
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (Barrel && isReloaded) 
	{
		auto socketRotation = Barrel->GetSocketRotation(FName("Projectile"));
		auto socketLocation = Barrel->GetSocketLocation(FName("Projectile"));

		auto projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			socketLocation,
			socketRotation
			);

		projectile->Launch(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}
