// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "exactoSim/ExactoPhysics.h"
#include "GameFramework/Actor.h"
#include "ExScene.generated.h"

UCLASS()
class EXACTOSIM_API AExScene : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<AActor*> DynObj;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExactoPhysics * ExPhyzX;
	AExScene();

	UFUNCTION(BlueprintCallable)
		void addSmplTestObject(FVector location, FRotator rotation);
	UFUNCTION(BlueprintCallable)
		void addGenerator(FVector location, FRotator rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void addObjByPath(FVector location, FRotator rotation, std::string path, std::string name);
};
