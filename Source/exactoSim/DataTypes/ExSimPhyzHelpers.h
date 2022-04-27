#pragma once

class EXACTOSIM_API ExSimPhyzHelpers
{
public:
	enum Constraint
	{
		HINGE = 0,
		HINGE2,
		GEN6DOF_SPRING,
		P2P,
		NONE
	};

	static FString getNameOfConstraint(Constraint Type)
	{
		if (Type == Constraint::GEN6DOF_SPRING)
			return TEXT("Generic 6DOF Spring");
		if (Type == Constraint::P2P)
			return TEXT("Point to Point");
		if (Type == Constraint::HINGE)
			return TEXT("Hinge");
		if (Type == Constraint::HINGE2)
			return TEXT("Hinge 2v");
		return TEXT("Unknown");
	}
};
