// Fill out your copyright notice in the Description page of Project Settings.


#include "ExactoPhysics.h"



// Sets default values
AExactoPhysics::AExactoPhysics()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BtMaxSubSteps = 1;
	BtPhysicsFrequency = 60.;


	// This is all pretty standard Bullet bootstrap
	BtCollisionConfig = new btDefaultCollisionConfiguration();
	BtCollisionDispatcher = new btCollisionDispatcher(BtCollisionConfig);
	BtBroadphase = new btDbvtBroadphase();
	BtConstraintSolver = new btSequentialImpulseConstraintSolver();
	BtWorld = new btDiscreteDynamicsWorld(BtCollisionDispatcher, BtBroadphase, BtConstraintSolver, BtCollisionConfig);

	// I mess with a few settings on BtWorld->getSolverInfo() but they're specific to my needs	

	// Gravity vector in our units (1=1cm)
	BtWorld->setGravity(BulletHelpers::ToBtDir(FVector(0, 0, -980)));
	
	// set up debug rendering
	BtDebugDraw = new BulletDebugDraw(GetWorld(), GetActorLocation());
	BtWorld->setDebugDrawer(BtDebugDraw);
}

AExactoPhysics::~AExactoPhysics()
{
	for (int i = BtWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		//btCollisionObject* obj = BtWorld->getCollisionObjectArray()[i];
		//if (obj)
		//{
		//	btRigidBody* body = btRigidBody::upcast(obj);
		//	if (body && body->getMotionState())
		//	{
		//		delete body->getMotionState();
		//	}
		//}
		//BtWorld->removeCollisionObject(obj);
		//	delete obj;
	}

	// delete collision shapes
	for (int i = 0; i < BtBoxCollisionShapes.Num(); i++)
		delete BtBoxCollisionShapes[i];
	BtBoxCollisionShapes.Empty();
	for (int i = 0; i < BtSphereCollisionShapes.Num(); i++)
		delete BtSphereCollisionShapes[i];
	BtSphereCollisionShapes.Empty();
	for (int i = 0; i < BtCapsuleCollisionShapes.Num(); i++)
		delete BtCapsuleCollisionShapes[i];
	BtCapsuleCollisionShapes.Empty();
	for (int i = 0; i < BtConvexHullCollisionShapes.Num(); i++)
		delete BtConvexHullCollisionShapes[i].Shape;
	BtConvexHullCollisionShapes.Empty();
	for (int i = 0; i < CachedDynamicShapes.Num(); i++)
	{
		// Only delete if this is a compound shape, otherwise it's an alias to other simple arrays
		if (CachedDynamicShapes[i].bIsCompound)
			delete CachedDynamicShapes[i].Shape;
	}
	CachedDynamicShapes.Empty();

	delete BtWorld;
	delete BtConstraintSolver;
	delete BtBroadphase;
	delete BtCollisionDispatcher;
	delete BtCollisionConfig;
	delete BtDebugDraw; // I haven't talked about this yet, later

	BtWorld = nullptr;
	BtConstraintSolver = nullptr;
	BtBroadphase = nullptr;
	BtCollisionDispatcher = nullptr;
	BtCollisionConfig = nullptr;
	BtDebugDraw = nullptr;

	// Clear our type-specific arrays (duplicate refs)
	BtStaticObjects.Empty();
	BtRigidBodies.Empty();
}
btRigidBody* AExactoPhysics::AddRigidBody(AActor* actor)
{
	if (actor != nullptr)
	{
		const CachedDynamicShapeData data = GetCachedDynamicShapeData(actor, 1.0);
		btRigidBody* body = AddRigidBody(actor, data, PhysicsStatic1Friction, PhysicsStatic1Restitution);
		return body;
	}
	return nullptr;
}

// Called when the game starts or when spawned
void AExactoPhysics::BeginPlay()
{
	Super::BeginPlay();
	SetupStaticGeometryPhysics(PhysicsStaticActors1, PhysicsStatic1Friction, PhysicsStatic1Restitution);

	// for (AActor* actor : DynamicActors)
	// {
	// 	if (actor != nullptr)
	// 	{
	// 		CachedDynamicShapeData data = GetCachedDynamicShapeData(actor, 1.0);
	// 		btRigidBody * body = AddRigidBody(actor, data, PhysicsStatic1Friction, PhysicsStatic1Restitution);
	// 	}
	// }	
	
}

// Called every frame
void AExactoPhysics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	btDispatcher * dp = BtWorld->getDispatcher();
	const int num_man = dp->getNumManifolds();
	for (int m = 0; m < num_man; m++)
	{
		btPersistentManifold* man = dp->getManifoldByIndexInternal( m );
		const int numc = man->getNumContacts();
		const btRigidBody* body_a = static_cast<const btRigidBody*>(man->getBody0());
		const btRigidBody * body_b = static_cast<const btRigidBody*> (man->getBody1());
		const btRigidBody* trg = nullptr;
		if (body_a == BtStaticObjects[0])
			trg = body_a;
		else if (body_b == BtStaticObjects[0])
			trg = body_b;
		if ((numc > 0)&&(trg != nullptr))
		{
			for (int c = 0; c < numc; ++c)
			{
				btManifoldPoint pt = man->getContactPoint(c);
				btVector3 r =  pt.getPositionWorldOnA();
				//UE_LOG(LogTemp, Warning, TEXT("Text, %f %f %f"), r.x() , r.y(), r.z()  );
				//здесь мы загружаем данные от датчиков и так далее
			}
			__nop();
		}
	}
	StepPhysics(DeltaTime);
}



void AExactoPhysics::StepPhysics(float DeltaSeconds)
{
	int a = 1;
	BtWorld->stepSimulation(DeltaSeconds, BtMaxSubSteps, 1. / BtPhysicsFrequency);

#if WITH_EDITORONLY_DATA
	if (bPhysicsShowDebug)
		BtWorld->debugDrawWorld();
#endif
}
btCollisionObject* AExactoPhysics::AddStaticCollision(btCollisionShape* Shape, const FTransform& Transform,
	float Friction, float Restitution, AActor* Actor)
{
	btTransform Xform = BulletHelpers::ToBt(Transform, GetActorLocation());
	btCollisionObject* Obj = new btCollisionObject();
	Obj->setCollisionShape(Shape);
	Obj->setWorldTransform(Xform);
	Obj->setFriction(Friction);
	Obj->setRestitution(Restitution);
	Obj->setUserPointer(Actor);

	Obj->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
	
	BtWorld->addCollisionObject(Obj);
	BtStaticObjects.Add(Obj);
	//if (TestCollider == nullptr)
	{
	//	TestCollider = new ExCollideResult(&OutputData);
		/*
		int flag = Obj->getCollisionFlags();
		flag |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
		Obj->setCollisionFlags(flag);
		ExCollideResult TestCollider(&OutputData);
		TestCollider.m_closestDistanceThreshold = 1;
		BtWorld->contactTest(Obj,TestCollider);
		*/
	}
	return Obj;
}

void AExactoPhysics::ExtractPhysicsGeometry(AActor* Actor, PhysicsGeometryCallback CB)
{
	TInlineComponentArray<UActorComponent*, 20> Components;
	// Used to easily get a component's transform relative to actor, not parent component
	const FTransform InvActorTransform = Actor->GetActorTransform().Inverse();

	// Collisions from meshes
	Actor->GetComponents(UStaticMeshComponent::StaticClass(), Components);
	for (auto&& Comp : Components)
	{
		ExtractPhysicsGeometry(Cast<UStaticMeshComponent>(Comp), InvActorTransform, CB);
	}
	// Collisions from separate collision components
	Actor->GetComponents(UShapeComponent::StaticClass(), Components);
	for (auto&& Comp : Components)
	{
		ExtractPhysicsGeometry(Cast<UShapeComponent>(Comp), InvActorTransform, CB);
	}
}

void AExactoPhysics::ExtractPhysicsGeometry(UStaticMeshComponent* SMC, const FTransform& InvActorXform,
	PhysicsGeometryCallback CB)
{
	UStaticMesh* Mesh = SMC->GetStaticMesh();
	if (!Mesh)
		return;

	// We want the complete transform from actor to this component, not just relative to parent
	FTransform CompFullRelXForm = SMC->GetComponentTransform() * InvActorXform;
	ExtractPhysicsGeometry(CompFullRelXForm, Mesh->GetBodySetup(), CB);

	// Not supporting complex collision shapes right now
	// If we did, note that Mesh->ComplexCollisionMesh is WITH_EDITORONLY_DATA so not available at runtime
	// See StaticMeshRender.cpp, FStaticMeshSceneProxy::GetDynamicMeshElements
	// Line 1417+, bDrawComplexCollision
	// Looks like we have to access LODForCollision, RenderData->LODResources
	// So they use a mesh LOD for collision for complex shapes, never drawn usually?
}

void AExactoPhysics::ExtractPhysicsGeometry(UShapeComponent* Sc, const FTransform& InvActorXform,
	PhysicsGeometryCallback CB)
{
	// We want the complete transform from actor to this component, not just relative to parent
	FTransform CompFullRelXForm = Sc->GetComponentTransform() * InvActorXform;
	ExtractPhysicsGeometry(CompFullRelXForm, Sc->ShapeBodySetup, CB);
}

void AExactoPhysics::ExtractPhysicsGeometry(const FTransform& XformSoFar, UBodySetup* BodySetup,
	PhysicsGeometryCallback CB)
{
	FVector Scale = XformSoFar.GetScale3D();
	btCollisionShape* Shape = nullptr;

	// Iterate over the simple collision shapes
	for (auto&& Box : BodySetup->AggGeom.BoxElems)
	{
		// We'll re-use based on just the LxWxH, including actor scale
		// Rotation and centre will be baked in world space
		FVector Dimensions = FVector(Box.X, Box.Y, Box.Z) * Scale;
		Shape = GetBoxCollisionShape(Dimensions);
		FTransform ShapeXform(Box.Rotation, Box.Center);
		// Shape transform adds to any relative transform already here
		FTransform XForm = ShapeXform * XformSoFar;
		CB(Shape, XForm);
	}
	for (auto&& Sphere : BodySetup->AggGeom.SphereElems)
	{
		// Only support uniform scale so use X
		Shape = GetSphereCollisionShape(Sphere.Radius * Scale.X);
		FTransform ShapeXform(FRotator::ZeroRotator, Sphere.Center);
		// Shape transform adds to any relative transform already here
		FTransform XForm = ShapeXform * XformSoFar;
		CB(Shape, XForm);
	}
	// Sphyl == Capsule (??)
	for (auto&& Capsule : BodySetup->AggGeom.SphylElems)
	{
		// X scales radius, Z scales height
		Shape = GetCapsuleCollisionShape(Capsule.Radius * Scale.X, Capsule.Length * Scale.Z);
		// Capsules are in Z in UE, in Y in Bullet, so roll -90
		FRotator Rot(0, 0, -90);
		// Also apply any local rotation
		Rot += Capsule.Rotation;
		FTransform ShapeXform(Rot, Capsule.Center);
		// Shape transform adds to any relative transform already here
		FTransform XForm = ShapeXform * XformSoFar;
		CB(Shape, XForm);
	}
	for (int i = 0; i < BodySetup->AggGeom.ConvexElems.Num(); ++i)
	{
		Shape = GetConvexHullCollisionShape(BodySetup, i, Scale);
		CB(Shape, XformSoFar);
	}
}

btCollisionShape* AExactoPhysics::GetBoxCollisionShape(const FVector& Dimensions)
{
	// Simple brute force lookup for now, probably doesn't need anything more clever
	btVector3 HalfSize = BulletHelpers::ToBtSize(Dimensions * 0.5);
	for (auto&& S : BtBoxCollisionShapes)
	{
		btVector3 Sz = S->getHalfExtentsWithMargin();
		if (FMath::IsNearlyEqual(Sz.x(), HalfSize.x()) &&
			FMath::IsNearlyEqual(Sz.y(), HalfSize.y()) &&
			FMath::IsNearlyEqual(Sz.z(), HalfSize.z()))
		{
			return S;
		}
	}

	// Not found, create
	auto S = new btBoxShape(HalfSize);
	// Get rid of margins, just cause issues for me
	S->setMargin(0);
	BtBoxCollisionShapes.Add(S);

	return S;
}

btCollisionShape* AExactoPhysics::GetSphereCollisionShape(float Radius)
{
	// Simple brute force lookup for now, probably doesn't need anything more clever
	btScalar Rad = BulletHelpers::ToBtSize(Radius);
	for (auto&& S : BtSphereCollisionShapes)
	{
		// Bullet subtracts a margin from its internal shape, so add back to compare
		if (FMath::IsNearlyEqual(S->getRadius(), Rad))
		{
			return S;
		}
	}

	// Not found, create
	auto S = new btSphereShape(Rad);
	// Get rid of margins, just cause issues for me
	S->setMargin(0);
	BtSphereCollisionShapes.Add(S);

	return S;
}

btCollisionShape* AExactoPhysics::GetCapsuleCollisionShape(float Radius, float Height)
{
	// Simple brute force lookup for now, probably doesn't need anything more clever
	btScalar R = BulletHelpers::ToBtSize(Radius);
	btScalar H = BulletHelpers::ToBtSize(Height);
	btScalar HalfH = H * 0.5f;

	for (auto&& S : BtCapsuleCollisionShapes)
	{
		// Bullet subtracts a margin from its internal shape, so add back to compare
		if (FMath::IsNearlyEqual(S->getRadius(), R) &&
			FMath::IsNearlyEqual(S->getHalfHeight(), HalfH))
		{
			return S;
		}
	}

	// Not found, create
	auto S = new btCapsuleShape(R, H);
	BtCapsuleCollisionShapes.Add(S);

	return S;
}

btCollisionShape* AExactoPhysics::GetConvexHullCollisionShape(UBodySetup* BodySetup, int ConvexIndex,
	const FVector& Scale)
{
	for (auto&& S : BtConvexHullCollisionShapes)
	{
		if (S.BodySetup == BodySetup && S.HullIndex == ConvexIndex && S.Scale.Equals(Scale))
		{
			return S.Shape;
		}
	}

	const FKConvexElem& Elem = BodySetup->AggGeom.ConvexElems[ConvexIndex];
	auto C = new btConvexHullShape();
	for (auto&& P : Elem.VertexData)
	{
		C->addPoint(BulletHelpers::ToBtPos(P, FVector::ZeroVector));
	}
	// Very important! Otherwise there's a gap between 
	C->setMargin(0);
	// Apparently this is good to call?
	C->initializePolyhedralFeatures();

	BtConvexHullCollisionShapes.Add({
		BodySetup,
		ConvexIndex,
		Scale,
		C
		});

	return C;
}

void AExactoPhysics::SetupStaticGeometryPhysics(TArray<AActor*> Actors, float Friction, float Restitution)
{
	for (AActor* Actor : Actors)
	{
		// Just in case we remove items from the list & leave blank
		if (Actor == nullptr)
			continue;

		ExtractPhysicsGeometry(Actor,
			[Actor, this, Friction, Restitution](btCollisionShape* Shape, const FTransform& RelTransform)
			{
				// Every sub-collider in the actor is passed to this callback function
				// We're baking this in world space, so apply actor transform to relative
				const FTransform FinalXform = RelTransform * Actor->GetActorTransform();
				AddStaticCollision(Shape, FinalXform, Friction, Restitution, Actor);
			});
	}
}

const AExactoPhysics::CachedDynamicShapeData& AExactoPhysics::GetCachedDynamicShapeData(AActor* Actor, float Mass)
{
	// We re-use compound shapes based on (leaf) BP class
	const FName ClassName = Actor->GetClass()->GetFName();
	for (auto&& Data : CachedDynamicShapes)
	{
		if (Data.ClassName == ClassName)
			return Data;
	}

	// Because we want to support compound colliders, we need to extract all colliders first before
	// constructing the final body.
	TArray<btCollisionShape*, TInlineAllocator<20>> Shapes;
	TArray<FTransform, TInlineAllocator<20>> ShapeRelXforms;
	ExtractPhysicsGeometry(Actor,
		[&Shapes, &ShapeRelXforms](btCollisionShape* Shape, const FTransform& RelTransform)
		{
			Shapes.Add(Shape);
			ShapeRelXforms.Add(RelTransform);
		});


	CachedDynamicShapeData ShapeData;
	ShapeData.ClassName = ClassName;

	// Single shape with no transform is simplest
	if (ShapeRelXforms.Num() == 1 &&
		ShapeRelXforms[0].EqualsNoScale(FTransform::Identity))
	{
		ShapeData.Shape = Shapes[0];
		// just to make sure we don't think we have to clean it up; simple shapes are already stored
		ShapeData.bIsCompound = false;
	}
	else
	{
		// Compound or offset single shape; we will cache these by blueprint type
		btCompoundShape* CS = new btCompoundShape();
		for (int i = 0; i < Shapes.Num(); ++i)
		{
			// We don't use the actor origin when converting transform in this case since object space
			// Note that btCompoundShape doesn't free child shapes, which is fine since they're tracked separately
			CS->addChildShape(BulletHelpers::ToBt(ShapeRelXforms[i], FVector::ZeroVector), Shapes[i]);
		}

		ShapeData.Shape = CS;
		ShapeData.bIsCompound = true;
	}

	// Calculate Inertia
	ShapeData.Mass = Mass;
	ShapeData.Shape->calculateLocalInertia(Mass, ShapeData.Inertia);

	// Cache for future use
	CachedDynamicShapes.Add(ShapeData);

	return CachedDynamicShapes.Last();
}

btRigidBody* AExactoPhysics::AddRigidBody(AActor* Actor, const CachedDynamicShapeData& ShapeData, float Friction,
	float Restitution)
{
	return AddRigidBody(Actor, ShapeData.Shape, ShapeData.Inertia, ShapeData.Mass, Friction, Restitution);
}

btRigidBody* AExactoPhysics::AddRigidBody(AActor* Actor, btCollisionShape* CollisionShape, btVector3 Inertia,
	float Mass, float Friction, float Restitution)
{
	auto Origin = GetActorLocation();
	auto MotionState = new BulletCustomMotionState(Actor, Origin);
	const btRigidBody::btRigidBodyConstructionInfo rbInfo(Mass, MotionState, CollisionShape, Inertia);
	btRigidBody* Body = new btRigidBody(rbInfo);
	Body->setUserPointer(Actor);
	Body->setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT|btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	BtWorld->addRigidBody(Body,btBroadphaseProxy::DefaultFilter, btBroadphaseProxy::AllFilter);


	BtRigidBodies.Add(Body);
	/*cbbbb = new ExCollideResult(&OutputData);
	cbbbb->m_closestDistanceThreshold = 0.1f;
	btCollisionObject * a = BtStaticObjects[0];
	btCollisionObject * b = BtRigidBodies[0];
	BtWorld->contactPairTest(a,b,*cbbbb);*/
	return Body;
}

void AExactoPhysics::AddComplexBody(TArray<ConnectedBodies> * system)
{
	//create all
	TArray<btRigidBody*> body_list;
	TArray<btTypedConstraint*> constr_list;
	for (ConnectedBodies & component : *system)
	{
		if (component.target != nullptr)
		{
			btRigidBody * one_body =  AddRigidBody(component.target);
			one_body->setActivationState(DISABLE_DEACTIVATION);
			body_list.Add(one_body);
			component.trg_body = one_body;
		}
	}
	//connect using list
	for ( auto& component : *system)
	{
		if (component.parent != nullptr)
		{
			for (auto & body : body_list)
			{
				AActor * parent = static_cast<AActor*>(body->getUserPointer());
				AActor * child = static_cast<AActor*>(component.trg_body->getUserPointer());
				if (parent == component.parent)
				{
					FString name_trg = child->GetName();
					FString name_par = parent->GetName();
					FString result = TEXT("Connect ") + name_trg + TEXT(" to ") + name_par;
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, result);

					btVector3 axisA = BulletHelpers::ToBtSize(component.axis_p);
					btVector3 axisB = BulletHelpers::ToBtSize(component.axis_t);
					btVector3 pivotA = BulletHelpers::ToBtSize(component.pivot_p);
					btVector3 pivotB = BulletHelpers::ToBtSize(component.pivot_t);
					if (component.constr_type == BulletHelpers::HINGE)
					{
						btHingeConstraint* p_hinge2 = new btHingeConstraint(*body, *component.trg_body, pivotA, pivotB, axisA, axisB);
						BtWorld->addConstraint(p_hinge2);
						p_hinge2->setDbgDrawSize(btScalar(5.f));
						constr_list.Add(p_hinge2);
						component.trg_constr = p_hinge2;

						p_hinge2->setLimit(0,0);
					}
					else if (component.constr_type == BulletHelpers::HINGE2)
					{
						btHinge2Constraint * p_hinge2 = new btHinge2Constraint(*body, *component.trg_body, pivotA, axisA, axisB);
						BtWorld->addConstraint(p_hinge2);
						constr_list.Add(p_hinge2);
						component.trg_constr = p_hinge2;						

						p_hinge2->enableMotor(0, true);
					}
					else if (component.constr_type == BulletHelpers::GEN6DOF_SPRING)
					{
						btTransform frame_a, frame_b;
						frame_a = btTransform::getIdentity();
						frame_a.setOrigin(pivotA);
						frame_b = btTransform::getIdentity();
						frame_b.setOrigin(pivotB);
						btGeneric6DofSpringConstraint * p_6dof_spring = new btGeneric6DofSpringConstraint(*body, *component.trg_body,
																frame_a, frame_b, true);
						p_6dof_spring->setLinearUpperLimit(BulletHelpers::ToBtSize(component.upp_lim_lin));
						p_6dof_spring->setLinearLowerLimit(BulletHelpers::ToBtSize(component.low_lim_lin));
						
					}
					else
					{
						btHingeConstraint* p_hinge2 = new btHingeConstraint(*body, *component.trg_body, pivotA, pivotB, axisA, axisB);
						BtWorld->addConstraint(p_hinge2);
						p_hinge2->setDbgDrawSize(btScalar(5.f));
						constr_list.Add(p_hinge2);
						component.trg_constr = p_hinge2;						
					}
				}
			}
		}
	}
	/*if (constr_list.Num() > 1)
	{
	btHingeConstraint * constr0 = static_cast<btHingeConstraint*>(constr_list[0]);
	constr0->enableAngularMotor(true, -1.f, 1.65f);
	constr0->setLimit(0.f,SIMD_HALF_PI);		
	btHingeConstraint * constr1 = static_cast<btHingeConstraint*>(constr_list[1]);
	constr1->setLimit(0.f,0.f);		
	}*/
}

void AExactoPhysics::removeRigidBody(btRigidBody* body)
{
	BtWorld->removeRigidBody(body);
	BtWorld->removeCollisionObject(body);
	BtRigidBodies.Remove(body);
	//delete body->getCollisionShape(); // <=== do not remove, используется далее при создании объектов
	delete body->getMotionState();
	delete body;
	
}

void AExactoPhysics::removeConstrain(btTypedConstraint* constr)
{
	BtWorld->removeConstraint(constr);
	delete constr;
}
