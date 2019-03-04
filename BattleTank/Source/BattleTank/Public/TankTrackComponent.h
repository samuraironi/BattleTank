// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "Components/SplineComponent.h"
#include "TankTrackComponent.generated.h"


class UInstancedStaticMeshComponent;
class USceneComponent;

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
	FORCEINLINE TArray<T*> GetWheels()
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

			Wheel->SetId(SpawnPointChild->GetId());
			ResultArray.Add(Wheel);
		}
		return ResultArray;
	};

	void SetupSpline(int wheelId, FVector location, float wheelRadius);
protected:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	int TreadCount = 80;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TrackTicknessOffset = 10;

	virtual void BeginPlay() override;
private:
	float DeltaX = 0;

	float TrackOffsetPercentage = 0;

	float LastLocation = 0.f;

	float CurrentThrottle = 0.f;

	UTankTrackComponent(const FObjectInitializer& objectInitializer);

	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Build(UInstancedStaticMeshComponent* mesh);

	UInstancedStaticMeshComponent* Mesh = nullptr;

	void DriveTrack();

	void MoveTrack();
};
