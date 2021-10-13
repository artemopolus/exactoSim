// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletDynamicComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXACTOSIM_API UBulletDynamicComponent : public UActorComponent
{
	GENERATED_BODY()

public: //variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet Physics")
	float Mass = 1.0;
public:	//functions
	// Sets default values for this component's properties
	UBulletDynamicComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
