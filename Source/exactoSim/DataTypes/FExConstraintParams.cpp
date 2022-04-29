#include "FExConstraintParams.h"

#include "exactoSim/Utils/ExConvert.h"

void ExConstraintDict::updateNames(TMap<EConstraintParamNames, FString>* trg, ExSimPhyzHelpers::Constraint type)
{
	if (trg->Num() > 0)
		trg->Empty();
	if (type == ExSimPhyzHelpers::Constraint::P2P)
	{
		trg->Add(EConstraintParamNames::parent_pivot,TEXT("1st Pivot"));
		trg->Add(EConstraintParamNames::target_pivot,TEXT("2st Pivot"));
		trg->Add(EConstraintParamNames::tau,TEXT("Tau"));
		trg->Add(EConstraintParamNames::impulse_clamp, TEXT("Impulse Clamp"));
		trg->Add(EConstraintParamNames::constraint_name,TEXT("P2P name"));
		return;
	}
	if (type == ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING)
	{
		trg->Add(EConstraintParamNames::parent_pivot,TEXT("Parent Pivot"));
		trg->Add(EConstraintParamNames::target_pivot,TEXT("Target Pivot"));
		trg->Add(EConstraintParamNames::low_lim_lin,TEXT("Linear Lower Limit"));
		trg->Add(EConstraintParamNames::upp_lim_lin,TEXT("Linear Upper Limit"));
		trg->Add(EConstraintParamNames::low_lim_ang,TEXT("Angular Lower Limit"));
		trg->Add(EConstraintParamNames::upp_lim_ang,TEXT("Angular Upper Limit"));
		trg->Add(EConstraintParamNames::stiff_lin,TEXT("Linear stiffness"));
		trg->Add(EConstraintParamNames::dump_lin,TEXT("Linear dumpling"));
		trg->Add(EConstraintParamNames::constraint_name,TEXT("G6DOF Spring name"));
		trg->Add(EConstraintParamNames::enables_spring, TEXT("Spring axis on"));
		return;
	}
}

void ExConstraintDict::updateValues(TMap<EConstraintParamNames, FString>* trg, FExConstraintParams* params)
{
	trg->FindOrAdd(EConstraintParamNames::parent_pivot) = ExConvert::getStrFromVec(params->pivot_p); 
	trg->FindOrAdd(EConstraintParamNames::target_pivot) = ExConvert::getStrFromVec(params->pivot_t); 
	trg->FindOrAdd(EConstraintParamNames::parent_axis) = ExConvert::getStrFromVec(params->axis_p); 
	trg->FindOrAdd(EConstraintParamNames::target_axis) = ExConvert::getStrFromVec(params->axis_t); 
	trg->FindOrAdd(EConstraintParamNames::dump_ang) = ExConvert::getStrFromVec(params->dump_ang); 
	trg->FindOrAdd(EConstraintParamNames::dump_lin) = ExConvert::getStrFromVec(params->dump_lin); 
	trg->FindOrAdd(EConstraintParamNames::low_lim_ang) = ExConvert::getStrFromVec(params->low_lim_ang); 
	trg->FindOrAdd(EConstraintParamNames::low_lim_lin) = ExConvert::getStrFromVec(params->low_lim_lin); 
	trg->FindOrAdd(EConstraintParamNames::stiff_ang) = ExConvert::getStrFromVec(params->stiff_ang); 
	trg->FindOrAdd(EConstraintParamNames::stiff_lin) = ExConvert::getStrFromVec(params->stiff_lin); 
	trg->FindOrAdd(EConstraintParamNames::upp_lim_ang) = ExConvert::getStrFromVec(params->upp_lim_ang); 
	trg->FindOrAdd(EConstraintParamNames::upp_lim_lin) = ExConvert::getStrFromVec(params->upp_lim_lin);
	
	trg->FindOrAdd(EConstraintParamNames::constraint_name) = params->name_constraint; 
	trg->FindOrAdd(EConstraintParamNames::parent_name) = params->name_p; 
	trg->FindOrAdd(EConstraintParamNames::target_name) = params->name_t; 

	trg->FindOrAdd(EConstraintParamNames::tau) = ExConvert::getStrFromFloat(params->tau);
	trg->FindOrAdd(EConstraintParamNames::impulse_clamp) = ExConvert::getStrFromFloat(params->impulse_clamp);

	trg->FindOrAdd(EConstraintParamNames::enables_spring) = ExConvert::getBoolStrFromInt(params->enables_spring, 6);

	trg->FindOrAdd(EConstraintParamNames::constraint_t) = ExSimPhyzHelpers::getNameFromConstraint(params->constr_type); 
	
}

void ExConstraintDict::getNameValuePairs(TMap<EConstraintParamNames, FString>* names,
	TMap<EConstraintParamNames, FString>* values, TMap<FString, FString>* trg)
{
	for(auto & n : *names)
	{
		FString name = n.Value;
		EConstraintParamNames c = n.Key;
		FString * value = values->Find(c);
		trg->FindOrAdd(name) = *value;
	}
}

