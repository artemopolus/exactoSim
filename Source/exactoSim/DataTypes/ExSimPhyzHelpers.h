#pragma once

#define ES_PHYZ_HELP_DEF_GENERIC6DOF_SPRING TEXT("Generic 6DOF Spring")
#define ES_PHYZ_HELP_DEF_POINT_TO_POINT TEXT("Point to Point")
#define ES_PHYZ_HELP_DEF_HINGE TEXT("Hinge")
#define ES_PHYZ_HELP_DEF_HINGE2 TEXT("Hinge 2v")
#define ES_PHYZ_HELP_DEF_NONE TEXT("Unknown")

enum class  EnExParamTypes : int
{
	CONSTRAINT = 0,
	COMPONENT,
	COMPLEX
};

class EXACTOSIM_API ExSimPhyzHelpers
{
public:
	enum Constraint
	{
		HINGE = 0,
		HINGE2,
		GEN6DOF_SPRING,
		P2P,
		GEAR,
		CONE_TWIST,
		UNI,
		SLIDER,
		NONE
	};

	static FString getNameFromConstraint(Constraint Type)
	{
		if (Type == Constraint::GEN6DOF_SPRING)
			return ES_PHYZ_HELP_DEF_GENERIC6DOF_SPRING;
		if (Type == Constraint::P2P)
			return ES_PHYZ_HELP_DEF_POINT_TO_POINT;
		if (Type == Constraint::HINGE)
			return ES_PHYZ_HELP_DEF_HINGE;
		if (Type == Constraint::HINGE2)
			return ES_PHYZ_HELP_DEF_HINGE2;
		return ES_PHYZ_HELP_DEF_NONE;
	}
	static Constraint getConstraintFromName(FString name)
	{
		if (name == ES_PHYZ_HELP_DEF_GENERIC6DOF_SPRING)
			return GEN6DOF_SPRING;
		if (name == ES_PHYZ_HELP_DEF_POINT_TO_POINT)
			return P2P;
		if (name == ES_PHYZ_HELP_DEF_HINGE)
			return HINGE;
		if (name == ES_PHYZ_HELP_DEF_HINGE2)
			return HINGE2;
		return NONE;
	}
	static TArray<FString> getConstraintsList(Constraint type)
	{
		TArray<FString> arr;
		arr.AddUnique(getNameFromConstraint(type));
		arr.AddUnique(ES_PHYZ_HELP_DEF_GENERIC6DOF_SPRING);
		arr.AddUnique(ES_PHYZ_HELP_DEF_HINGE);
		arr.AddUnique(ES_PHYZ_HELP_DEF_HINGE2);
		arr.AddUnique(ES_PHYZ_HELP_DEF_POINT_TO_POINT);
		return arr;
	}
};
