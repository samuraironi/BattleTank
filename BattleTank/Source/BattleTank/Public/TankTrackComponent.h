// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "TankTrackComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrackComponent : public USplineComponent
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = Input)
		void SetThrottle(float throttle);

	//Max force pert track in Newtons
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 400000; //Assume 40 tonne tank and 1g acceleration

	template<class T>
	FORCEINLINE TArray<T*> GetWheels() const;
private:
	UTankTrackComponent();

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* Mesh = nullptr;

	void DriveTrack(float CurrentThrottle);
};
