// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExScene.h"
#include "GameFramework/Actor.h"
#include "ExSmplBox.h"
#include "exactoSim/ExactoPhysics.h"
#include "ExGenerator.generated.h"

UCLASS()
class EXACTOSIM_API AExGenerator : public AActor
{
	GENERATED_BODY()


public:
	AExGenerator();

	virtual void generateObj();
	virtual void generateObj(FVector impulse);
	void setGeneratedObjPrefix(std::string name);
	std::string getGeneratedObjPrefix();
	void setGenObjInfo(AExScene::actor_info info);
	void setGenObjInfo(std::string name);
private:
	std::string getPathTo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	int GenObjectCount = 0;
	std::string GeneratedObjPrefix;
	AExScene::actor_info GenObjInfo;

public:
	// Sets default values for this actor's properties
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<AExSmplBox*> targets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExactoPhysics * ExPhyzX;*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		AExScene * ParentScene;
};
