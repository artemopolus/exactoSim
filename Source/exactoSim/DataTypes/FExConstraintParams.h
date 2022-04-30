#pragma once
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

#include "ExSimPhyzHelpers.h"
#include "exactoSim/BulletSpec/BulletHelpers.h"
#include "FExConstraintParams.generated.h"

USTRUCT()
struct FExConstraintParams
{
	GENERATED_BODY()
	
	btTypedConstraint* constraint;
	AActor* parent;

	FVector axis_t;
	FVector axis_p;
	FVector pivot_t;
	FVector pivot_p;

	FVector upp_lim_lin;
	FVector low_lim_lin;
	FVector upp_lim_ang;
	FVector low_lim_ang;

	FVector stiff_lin;
	FVector stiff_ang;

	FVector dump_lin;
	FVector dump_ang;

	FString name_p;
	FString name_t;
	FString name_constraint;
	ExSimPhyzHelpers::Constraint constr_type;

	float tau;
	float impulse_clamp;
	int enables_spring;

	FExConstraintParams()
	{
		constraint = nullptr;
		parent = nullptr;
		constr_type = ExSimPhyzHelpers::Constraint::NONE;

		axis_p = axis_t = pivot_t = pivot_p = FVector::ZeroVector;
		name_p = name_t = "Default";
		upp_lim_ang = low_lim_lin = upp_lim_lin = low_lim_ang = FVector::ZeroVector;
		stiff_ang = stiff_lin = dump_ang = dump_lin = FVector::ZeroVector;
		tau = 0; impulse_clamp = 0;
		enables_spring = 0;
	}
};

enum class EConstraintParamNames : int
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
	constraint_name,
	//
	spec_start,
	en_spring,
	constraint_t,
	//
	float_start,
	tau,
	impulse_clamp,
	//
	int_start,
	enables_spring,
	opt_end
};

class EXACTOSIM_API ExConstraintDict
{
public:
	static void updateNames(TMap<EConstraintParamNames, FString> * trg, ExSimPhyzHelpers::Constraint type);
	static void updateValues(TMap<EConstraintParamNames, FString> * trg, FExConstraintParams * params);
	static void getNameValuePairs(TMap<EConstraintParamNames, FString> * names, TMap<EConstraintParamNames, FString> * values, TMap<FString, FString> * trg);
};

