#pragma once
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

#include "ExCommonParams.h"
#include "ExSimPhyzHelpers.h"
#include "exactoSim/BulletSpec/BulletHelpers.h"
#include "FExConstraintParams.generated.h"

USTRUCT()
struct FExConstraintParams : public FExCommonParams
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
	float lower_limit;
	float upper_limit;
	
	int enables_spring;
private:
	ExSimObject * additionalPointer;
public:

	FExConstraintParams()
	{
		constraint = nullptr;
		parent = nullptr;
		constr_type = ExSimPhyzHelpers::Constraint::NONE;

		axis_p = axis_t = pivot_t = pivot_p = FVector::ZeroVector;
		name_p = name_t = name_constraint = TEXT("Default");
		upp_lim_ang = low_lim_lin = upp_lim_lin = low_lim_ang = FVector::ZeroVector;
		stiff_ang = stiff_lin = dump_ang = dump_lin = FVector::ZeroVector;
		tau = impulse_clamp = lower_limit = upper_limit = 0.f;
		enables_spring = 0;
		setType(EnExParamTypes::CONSTRAINT);
	}
	void setAdditionalPointer( ExSimObject * data){additionalPointer = data;}
	ExSimObject * getAdditionalPointer()
	{
		ExSimObject * tmp = additionalPointer;
		additionalPointer = nullptr;
		return tmp;
	}
};

enum class EnExConstraintParamNames : int32
{
	//fvectors
	VECTOR_START = 0,
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
	STRING_START,
	parent_name,
	target_name,
	constraint_name,
	//
	SPEC_START,
	en_spring,
	constraint_t,
	//
	FLOAT_START,
	tau,
	impulse_clamp,
	lower_limit,
	upper_limit,
	//
	INT_START,
	enables_spring,
	opt_end
};

class EXACTOSIM_API ExConstraintDict
{
public:
	static void updateNames(TMap<EnExConstraintParamNames, FString> * trg, ExSimPhyzHelpers::Constraint type);
	static void updateValues(TMap<EnExConstraintParamNames, FString> * trg, FExConstraintParams * params);
	static void getDefaultNames(TMap<EnExConstraintParamNames, FString> * trg);
	static void getNameValuePairs(TMap<EnExConstraintParamNames, FString> * names, TMap<EnExConstraintParamNames, FString> * values, TMap<FString, FString> * trg);
	static void fromNameValuePairsToParams(TMap<FString, FString> * src, FExConstraintParams * trg);

	static void getInitValues(TMap<EnExConstraintParamNames, TArray<FString>>* trg, FExConstraintParams* params);
	static void getEditTypeCreate(TMap<EnExConstraintParamNames, EnExParamEdit>* trg);
};

