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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SpawnGeneratorLoc = FVector(0,50,300);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SpawnObjectLoc = FVector(0, -100, 200);
	struct actor_cmd
	{
		int value_int;
		float value_float;
		std::string value_str;
	};
	
	AExScene();

	UFUNCTION(BlueprintCallable)
		void addSmplTestObject(FVector location, FRotator rotation);
	UFUNCTION(BlueprintCallable)
		void addGenerator(FVector location, FRotator rotation);
	UFUNCTION(BlueprintCallable)
		void addCarGen(FVector location, FRotator rotation);
	UFUNCTION(BlueprintCallable)
		void sendCmdToSelected(int type, float value);

	void sendExtendedCmdToSelected( actor_cmd cmd);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void addObjByPath(FVector location, FRotator rotation, std::string path, std::string name);
	void addObjByPath(FVector location, FRotator rotation, std::string path, std::string name, FVector impulse);

	void deleteSceneObjByPrefix(std::string prefix);

	void generateCar();
	
private:
	int ValueInt;
	float ValueFloat;
	std::string ValueStr;
	struct actor_body_storage
	{
		AActor * actor;
		btRigidBody * body;
		friend bool operator==(const actor_body_storage& left, const actor_body_storage& right)
		{
			return (left.actor == right.actor)&&(left.body == right.body);
		}
	};
	TArray<actor_body_storage> SceneObjects;
public:
	struct actor_info
	{
		std::string name;
		std::string name_prefix;
		bool enabled;
		std::string prefix;
		std::string folder;
		int genobjcnt;
		std::string genobjname;
		actor_info & operator = ( const actor_info & right)
		{
			this->name = right.name;
			this->enabled = right.enabled;
			this->prefix = right.prefix;
			this->folder = right.folder;
			this->prefix = right.prefix;
			return *this;
		}
	};



};
