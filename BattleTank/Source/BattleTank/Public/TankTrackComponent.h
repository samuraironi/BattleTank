// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "TankTrackComponent.generated.h"


class UInstancedStaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrackComponent : public USplineComponent
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = Input)
		void SetThrottle(float throttle);

	//Max force pert track in Newtons
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 4000000; //Assume 40 tonne tank and 1g acceleration

	template<class T>
	FORCEINLINE TArray<T*> GetWheels() const;

	template<class T>
	TArray<T*> GetChildrenWheels() const;

	void SetupSpline(int wheelId, FVector location, float wheelRadius);

	void AddForce(float ForceMagnitude);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	int TreadCount = 80;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TrackTicknessOffset = 10;

private:
	UTankTrackComponent(const FObjectInitializer& objectInitializer);

	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	float TotalForceMagnitudeThisFrame = 0;

	float TrackOffsetPercentage = 0;

	UFUNCTION(BlueprintCallable)
	void Build(UInstancedStaticMeshComponent* mesh);

	UInstancedStaticMeshComponent* Mesh = nullptr;

	void DriveTrack(float CurrentThrottle);

	void MoveTrack();
};
