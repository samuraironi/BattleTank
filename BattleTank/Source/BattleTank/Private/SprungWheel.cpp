// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/World.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	bHidden = false;
	bHiddenEd = false;

	BuildWheel();
}

void ASprungWheel::BuildWheel()
{
	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	SetRootComponent(MassWheelConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassWheelConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("WheelMesh"));
	WheelMesh->SetupAttachment(Wheel);

	WheelMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	WheelMesh->SetWorldScale3D(FVector(0.8f));


	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Axle);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
	//notify on component hit
	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraints();
}


void ASprungWheel::SetupConstraints()
{
	if (!GetAttachParentActor()) { return; }

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!BodyRoot) { return; }
	MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("force per wheel %f"), TotalForceMagnitudeThisFrame);
	ApplyForce();

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0;
	}
}

void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	WheelHitDelegate.ExecuteIfBound();
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

float ASprungWheel::GetRadius()
{
	return  Wheel->GetScaledSphereRadius();
}

FVector ASprungWheel::GetLocation()
{
	return Wheel->GetComponentLocation();
}

int ASprungWheel::GetId()
{
	return this->Id;
}

void ASprungWheel::SetId(int id)
{
	Id = id;
}

void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

void ASprungWheel::DriveWheel(float relativeSpeed)
{
	relativeSpeed = FMath::Clamp<float>(relativeSpeed, -1, +1);
	auto rotationChange = relativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	if (relativeSpeed == 0) return;

	MaxDegreesPerSecond++;

	if (CurrentWheelPos >= 360)
		CurrentWheelPos = 0;
	else
		CurrentWheelPos += rotationChange;

	Wheel->SetRelativeRotation(FRotator(CurrentWheelPos, 0, 0));
}

