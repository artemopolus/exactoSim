// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "exactoSim/Common/ExSimStorage.h"
#include "GameFramework/Actor.h"
#include "ExSmplBox.generated.h"

UCLASS()
class EXACTOSIM_API AExSmplBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AActor * RenBody;
	AExSmplBox();
	void setEScomponent(ExSimComponent * trg);
	ExSimComponent * getEScomponent();
	
private:
	ExSimComponent * ESCompPtr = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
