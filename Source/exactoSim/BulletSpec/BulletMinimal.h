#pragma once
#include "CoreMinimal.h"
#include "Windows/WindowsPlatformCompilerPreSetup.h"


THIRD_PARTY_INCLUDES_START
// This is needed to fix memory alignment issues
PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
// Value types
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

// Core things we override
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btIDebugDraw.h>

// Forward decl for everything else we use
class btCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btDynamicsWorld;
class btCollisionShape;
class btBoxShape;
class btCapsuleShape;
class btConvexHullShape;
class btCompoundShape;
class btSphereShape;
class btRigidBody;
class btCollisionObject;
class btCollisionWorld;

PRAGMA_POP_PLATFORM_DEFAULT_PACKING
THIRD_PARTY_INCLUDES_END

