// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnPoint.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API USpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnPoint();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* GetSpawnedActor() const { return SpawnedActor; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
	virtual void OnRegister() override;
private:
	UPROPERTY(EditAnywhere, Category = Setup)
	int Id = 0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY()
	AActor* SpawnedActor;

	UFUNCTION(BlueprintCallable)
	void SetSpawnActor();
};
