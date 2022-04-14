// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <functional>
//Bullet
#include "BulletSpec/BulletMinimal.h"
#include "BulletSpec/BulletHeader.h"
#include "Components/ShapeComponent.h"
#include "BulletSpec/BulletHelpers.h"
#include "BulletSpec/BulletCustomMotionState.h"
#include "BulletSpec/BulletDebugDraw.h"
#include "BulletSpec/BulletDynamicComponent.h"
#include "Scene/ExCollideResult.h"
#include "ExactoPhysics.generated.h"

UCLASS()
class EXACTOSIM_API AExactoPhysics : public AActor
{
	GENERATED_BODY()
	
public:	//variables
	//struct for body constructor
	struct ConnectedBodies
	{
		AActor * target;
		AActor * parent;
		btRigidBody * trg_body;
		btTypedConstraint * trg_constr;
		FVector axis_t;
		FVector axis_p;
		FVector pivot_t;
		FVector pivot_p;
		FString name_t;
		FString name_p;
		BulletHelpers::Constr constr_type;
		uint8_t en_spring[6];
		FVector upp_lim_lin;
		FVector low_lim_lin;
		FVector upp_lim_ang;
		FVector low_lim_ang;

		FVector stiff_lin;
		FVector stiff_ang;

		FVector dump_lin;
		FVector dump_ang;
	};
	enum es_options_list
	{
		//fvectors
		vector_start = 0,
		parent_pivot,
		target_pivot,
		upp_lim_lin,
		low_lim_lin,
		upp_lim_ang,
		low_lim_ang,
		stiff_lin,
		stiff_ang,
		dump_lin,
		dump_ang,
		parent_axis,
		target_axis,
		//fstrings
		string_start,
		parent_name,
		target_name,
		//
		spec_start,
		en_spring,
		constraint_t,
		//
		opt_end
	};	
	struct es_constraint
	{
		btTypedConstraint * constraint;
		AActor * parent;
    	BulletHelpers::Constr constr_type;
		
		FVector axis_t;
    	FVector axis_p;
    	FVector pivot_t;
    	FVector pivot_p;
		
    	FString name_t;
    	FString name_p;
    	uint8_t en_spring[6];
		TArray<bool> enable_spring;
    	FVector upp_lim_lin;
    	FVector low_lim_lin;
    	FVector upp_lim_ang;
    	FVector low_lim_ang;
    
    	FVector stiff_lin;
    	FVector stiff_ang;
    
    	FVector dump_lin;
    	FVector dump_ang;
		es_constraint()
		{
			constraint = nullptr;
			parent = nullptr;
			constr_type = BulletHelpers::Constr::NONE;
			en_spring[0] = 0;	en_spring[1] = 0;	en_spring[2] = 0;	en_spring[3] = 0;	en_spring[4] = 0;	en_spring[5] = 0;	
			axis_p =  axis_t =  pivot_t =  pivot_p = FVector::ZeroVector;
			name_p = name_t = "";
			upp_lim_ang = low_lim_lin = upp_lim_lin = low_lim_ang = FVector::ZeroVector;
			stiff_ang = stiff_lin = dump_ang = dump_lin = FVector::ZeroVector;
		}
	};	
	// Global objects
	btCollisionConfiguration* BtCollisionConfig;
	btCollisionDispatcher* BtCollisionDispatcher;
	btBroadphaseInterface* BtBroadphase;
	btConstraintSolver* BtConstraintSolver;
	btDynamicsWorld* BtWorld;
	// Custom debug interface
	btIDebugDraw* BtDebugDraw;
	// Dynamic bodies
	TArray<btRigidBody*> BtRigidBodies;
	// Static colliders
	TArray<btCollisionObject*> BtStaticObjects;
	// Re-usable collision shapes
	TArray<btBoxShape*> BtBoxCollisionShapes;
	TArray<btSphereShape*> BtSphereCollisionShapes;
	TArray<btCapsuleShape*> BtCapsuleCollisionShapes;

	int BtMaxSubSteps;
	float BtPhysicsFrequency;
	// Structure to hold re-usable ConvexHull shapes based on origin BodySetup / subindex / scale
	struct ConvexHullShapeHolder
	{
		UBodySetup* BodySetup;
		int HullIndex;
		FVector Scale;
		btConvexHullShape* Shape;
	};
	TArray<ConvexHullShapeHolder> BtConvexHullCollisionShapes;
	// These shapes are for *potentially* compound rigid body shapes
	struct CachedDynamicShapeData
	{
		FName ClassName; // class name for cache
		btCollisionShape* Shape;
		bool bIsCompound; // if true, this is a compound shape and so must be deleted
		btScalar Mass;
		btVector3 Inertia; // because we like to precalc this
	};
	TArray<CachedDynamicShapeData> CachedDynamicShapes;
	// This list can be edited in the level, linking to placed static actors
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet Physics|Objects")
		TArray<AActor*> PhysicsStaticActors1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet Physics|Objects")
		TArray<AActor*> DynamicActors;
	// These properties can only be edited in the Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet Physics|Objects")
		float PhysicsStatic1Friction = 0.6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet Physics|Objects")
		float PhysicsStatic1Restitution = 0.3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet Physics|Options")
		bool bPhysicsShowDebug = false;
	// I chose not to use spinning / rolling friction in the end since it had issues!
private:
	//ExCollideResult * TestCollider = nullptr;
	ExCollideResult * cbbbb = nullptr;
	ExSensorData OutputData;
public: //functions
	// Sets default values for this actor's properties
	AExactoPhysics();
	~AExactoPhysics();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StepPhysics(float DeltaSeconds);

	btCollisionObject* AddStaticCollision(btCollisionShape* Shape, const FTransform& Transform, float Friction,
		float Restitution, AActor* Actor);

	typedef const std::function<void(btCollisionShape* /*SingleShape*/, const FTransform& /*RelativeXform*/)>& PhysicsGeometryCallback;

	void ExtractPhysicsGeometry(AActor* Actor, PhysicsGeometryCallback CB);
	void ExtractPhysicsGeometry(UStaticMeshComponent* SMC, const FTransform& InvActorXform, PhysicsGeometryCallback CB);
	void ExtractPhysicsGeometry(UShapeComponent* Sc, const FTransform& InvActorXform, PhysicsGeometryCallback CB);
	void ExtractPhysicsGeometry(const FTransform& XformSoFar, UBodySetup* BodySetup, PhysicsGeometryCallback CB);


	btCollisionShape* GetBoxCollisionShape(const FVector& Dimensions);
	btCollisionShape* GetSphereCollisionShape(float Radius);
	btCollisionShape* GetCapsuleCollisionShape(float Radius, float Height);
	btCollisionShape* GetConvexHullCollisionShape(UBodySetup* BodySetup, int ConvexIndex, const FVector& Scale);

	void SetupStaticGeometryPhysics(TArray<AActor*> Actors, float Friction, float Restitution);
	const CachedDynamicShapeData& GetCachedDynamicShapeData(AActor* Actor, float Mass);

	btRigidBody* AddRigidBody(AActor* Actor, float Mass = 1.0f);
	btRigidBody* AddRigidBody(AActor* Actor, const CachedDynamicShapeData& ShapeData, float Friction, float Restitution);
	btRigidBody* AddRigidBody(AActor* Actor, btCollisionShape* CollisionShape, btVector3 Inertia, float Mass, float Friction, float Restitution);

	void AddComplexBody(TArray<ConnectedBodies> * system);

	btTypedConstraint * createConstraint(btRigidBody * target, btRigidBody * parent, es_constraint params);

	void removeRigidBody(btRigidBody * body);
	void removeConstrain(btTypedConstraint * constr);
};
