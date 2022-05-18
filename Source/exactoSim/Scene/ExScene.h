// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "exactoSim/ExactoPhysics.h"
#include "exactoSim/DataTypes/ExSimComponent.h"
#include "GameFramework/Actor.h"
#include "ExScene.generated.h"

class AExGenerator;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
		FString BaseTag = "Spawned";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
		FString PhysicsTag = "Bullet";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
		FString DynamicTag = "Dynamic";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
		FString StaticTag = "Static";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
		FString GeneratorTag = "Generator";	
	struct actor_cmd
	{
		int value_int;
		float value_float;
		std::string value_str;
	};
	
	AExScene();

	UFUNCTION(BlueprintCallable)
		void addSmplTestObject(FString name, float mass = 1.0f, FVector location = FVector(0,0,0), FRotator rotation = FRotator(0,0,0));
	UFUNCTION(BlueprintCallable)
		void addGenerator(FVector location, FRotator rotation);
	UFUNCTION(BlueprintCallable)
		void addCarGen(FVector location, FRotator rotation);
	UFUNCTION(BlueprintCallable)
		void sendCmdToSelected(int type, float value);

	UFUNCTION(BlueprintCallable)
		void moveGenerator(FVector loc, FRotator rot);

	void sendExtendedCmdToSelected( actor_cmd cmd);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void addObjByPath( FVector location, FRotator rotation, std::string path, std::string name);
	void addObjByPath( FVector location, FRotator rotation, std::string path, std::string name, FVector impulse);
	bool addObjByPath( const FString path, const FString name, btRigidBody ** body, float mass = 1.0f,
		FVector location = FVector(0,0,0), FRotator rotation = FRotator(0,0,0), bool use_genloc = true,
		FVector impulse = FVector(0,0,0), FVector impulse_pos = FVector(0,0,0));

	bool addObjByPath(ExSimComponent * component);
	bool addObjByPath(ExSimComponent ** component, const FString path, const FString name, float mass = 1.0f,
		FVector location = FVector(0,0,0), FRotator rotation = FRotator(0,0,0), bool use_genloc = true,
		FVector impulse = FVector(0,0,0), FVector impulse_pos = FVector(0,0,0));
	
	
	void deleteSceneObjByPrefix(std::string prefix);

	void generateCar();
	void removeCar();

	void createComponent(ExSimComponent * component);
	void updateComponent(ExSimComponent * component);

	void createConstraint(btRigidBody * target, btRigidBody * parent, FExConstraintParams params);
	void removeConstrain();

	void updateConstraint(btPoint2PointConstraint * c, FExConstraintParams * params);
	void updateConstraint(ExSimConstraintPair * pair);

	btTypedConstraint * fixP2P(btRigidBody * body, FVector location);
	btTypedConstraint * fixP2P(btRigidBody * body, FExConstraintParams * params);
	
	ExSimConstraintPair * fixP2P(ExSimComponent * component, FExConstraintParams * params);
	ExSimConstraintPair * fixGen6dofSpring(ExSimComponent* comp_a, ExSimComponent* comp_b, FExConstraintParams* params);
	ExSimConstraintPair * fixHinge2Constraint(ExSimComponent * par, ExSimComponent * trg, FExConstraintParams * params);	
	ExSimConstraintPair * fixHinge1Constraint(ExSimComponent * par, ExSimComponent * trg, FExConstraintParams * params);
	ExSimConstraintPair * fixUniConstraint(ExSimComponent * par, ExSimComponent * trg, FExConstraintParams * params);
	ExSimConstraintPair * fixConeTwist(ExSimComponent * par, ExSimComponent * trg, FExConstraintParams * params);
	ExSimConstraintPair * fixSlider(ExSimComponent * par, ExSimComponent * trg, FExConstraintParams * params);
	ExSimConstraintPair * fixGear(ExSimComponent * par, ExSimComponent * trg, FExConstraintParams * params);

	void fixP2P(ExSimConstraintPair * in, ExSimComponent * component);
	void fixGen6dofSpring(ExSimConstraintPair * in, ExSimComponent* par, ExSimComponent* trg);
	void fixHinge2Constraint(ExSimConstraintPair * in, ExSimComponent* par, ExSimComponent* trg);	
	void fixHinge1Constraint(ExSimConstraintPair * in, ExSimComponent* par, ExSimComponent* trg);
	void fixUniConstraint(ExSimConstraintPair * in, ExSimComponent* par, ExSimComponent* trg);
	void fixConeTwist(ExSimConstraintPair * in, ExSimComponent* par, ExSimComponent* trg);
	void fixSlider(ExSimConstraintPair * in, ExSimComponent* par, ExSimComponent* trg);
	void fixGear(ExSimConstraintPair * in, ExSimComponent* par, ExSimComponent* trg);

	ExSimConstraintPair * createConstraint(ExSimComponent * par, ExSimComponent * trg, FExConstraintParams * params);
	void createConstraint(ExSimConstraintPair * in, ExSimComponent * par, ExSimComponent * trg);
	bool checkConstraint(ExSimConstraintPair * trg);
	
	btTypedConstraint * fixGen6dofSpring(btRigidBody * p_body_a, btRigidBody * p_body_b, FExConstraintParams params);
	void pickTrgBody(btRigidBody * body, FVector location);
	void pickTrgBody(ExSimComponent * component, FVector location);
	void moveTrgBody(FVector location);
	void letTrgBody();
	bool getTrgBody(AActor ** actor);
	
	
private:
	btTypedConstraint * PickConstraint;
	btRigidBody *PickedBody;
	int PickState;
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
	TArray<TArray<AExactoPhysics::ConnectedBodies>> SystemsList;	
	enum stage
	{
		NONE,
		BREAK,
		REMOVE
	};
	stage CurrentStage = stage::NONE;
	float Time = 0;

	AExGenerator * CurrentGenerator = nullptr;
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
