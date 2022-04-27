#include "ExCollideResult.h"


bool ExCollideResult::needsCollision(btBroadphaseProxy* proxy0) const
{
	return true;
}

btScalar ExCollideResult::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0,
                                          int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	__nop();
	context->X = 56;
	return 0;
}
