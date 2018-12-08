// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	//auto time = GetWorld()->GetTimeSeconds();
	auto name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);
}


