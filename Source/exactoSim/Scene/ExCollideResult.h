#pragma once

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include "../BulletSpec/BulletMinimal.h"

struct ExSensorData
{
	int X;
	int Y;
	int Z;
};


struct ExCollideResult : public btCollisionWorld::ContactResultCallback
{
	ExCollideResult(ExSensorData * ptr) : context(ptr)
	{
		__nop();
		context->X = 0;
		m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
		m_collisionFilterMask = btBroadphaseProxy::AllFilter;
	}

	virtual bool needsCollision(btBroadphaseProxy* proxy0) const override;

	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
	                                 const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;
	ExSensorData * context;
};
