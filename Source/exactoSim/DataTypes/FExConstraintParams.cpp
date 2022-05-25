#include "FExConstraintParams.h"

#include "exactoSim/Utils/ExConvert.h"

void ExConstraintDict::updateNames(TMap<EnExConstraintParamNames, FString>* trg, ExSimPhyzHelpers::Constraint type)
{
	if (trg->Num() > 0)
		trg->Empty();
	if (type == ExSimPhyzHelpers::Constraint::P2P)
	{
		trg->Add(EnExConstraintParamNames::parent_pivot,TEXT("1st Pivot"));
		trg->Add(EnExConstraintParamNames::target_pivot,TEXT("2st Pivot"));
		trg->Add(EnExConstraintParamNames::tau,TEXT("Tau"));
		trg->Add(EnExConstraintParamNames::impulse_clamp, TEXT("Impulse Clamp"));
		trg->Add(EnExConstraintParamNames::constraint_name,TEXT("P2P name"));
		return;
	}
	if (type == ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING)
	{
		trg->Add(EnExConstraintParamNames::parent_pivot,TEXT("Parent Pivot"));
		trg->Add(EnExConstraintParamNames::target_pivot,TEXT("Target Pivot"));
		trg->Add(EnExConstraintParamNames::low_lim_lin,TEXT("Linear Lower Limit"));
		trg->Add(EnExConstraintParamNames::upp_lim_lin,TEXT("Linear Upper Limit"));
		trg->Add(EnExConstraintParamNames::low_lim_ang,TEXT("Angular Lower Limit"));
		trg->Add(EnExConstraintParamNames::upp_lim_ang,TEXT("Angular Upper Limit"));
		trg->Add(EnExConstraintParamNames::stiff_lin,TEXT("Linear stiffness"));
		trg->Add(EnExConstraintParamNames::dump_lin,TEXT("Linear dumpling"));
		trg->Add(EnExConstraintParamNames::constraint_name,TEXT("G6DOF Spring name"));
		trg->Add(EnExConstraintParamNames::enables_spring, TEXT("Spring axis on"));
		return;
	}
}

void ExConstraintDict::updateValues(TMap<EnExConstraintParamNames, FString>* trg, FExConstraintParams* params)
{
	trg->FindOrAdd(EnExConstraintParamNames::parent_pivot) = ExConvert::getStrFromVec(params->pivot_p); 
	trg->FindOrAdd(EnExConstraintParamNames::target_pivot) = ExConvert::getStrFromVec(params->pivot_t); 
	trg->FindOrAdd(EnExConstraintParamNames::parent_axis) = ExConvert::getStrFromVec(params->axis_p); 
	trg->FindOrAdd(EnExConstraintParamNames::target_axis) = ExConvert::getStrFromVec(params->axis_t); 
	trg->FindOrAdd(EnExConstraintParamNames::dump_ang) = ExConvert::getStrFromVec(params->dump_ang); 
	trg->FindOrAdd(EnExConstraintParamNames::dump_lin) = ExConvert::getStrFromVec(params->dump_lin); 
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_ang) = ExConvert::getStrFromVec(params->low_lim_ang); 
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_lin) = ExConvert::getStrFromVec(params->low_lim_lin); 
	trg->FindOrAdd(EnExConstraintParamNames::stiff_ang) = ExConvert::getStrFromVec(params->stiff_ang); 
	trg->FindOrAdd(EnExConstraintParamNames::stiff_lin) = ExConvert::getStrFromVec(params->stiff_lin); 
	trg->FindOrAdd(EnExConstraintParamNames::upp_lim_ang) = ExConvert::getStrFromVec(params->upp_lim_ang); 
	trg->FindOrAdd(EnExConstraintParamNames::upp_lim_lin) = ExConvert::getStrFromVec(params->upp_lim_lin);
	
	trg->FindOrAdd(EnExConstraintParamNames::constraint_name) = params->name_constraint; 
	trg->FindOrAdd(EnExConstraintParamNames::parent_name) = params->name_p; 
	trg->FindOrAdd(EnExConstraintParamNames::target_name) = params->name_t; 

	trg->FindOrAdd(EnExConstraintParamNames::tau) = ExConvert::getStrFromFloat(params->tau);
	trg->FindOrAdd(EnExConstraintParamNames::impulse_clamp) = ExConvert::getStrFromFloat(params->impulse_clamp);
	trg->FindOrAdd(EnExConstraintParamNames::lower_limit) = ExConvert::getStrFromFloat(params->lower_limit);
	trg->FindOrAdd(EnExConstraintParamNames::upper_limit) = ExConvert::getStrFromFloat(params->upper_limit);

	trg->FindOrAdd(EnExConstraintParamNames::enables_spring) = ExConvert::getBoolStrFromInt(params->enables_spring, 6);

	trg->FindOrAdd(EnExConstraintParamNames::constraint_t) = ExSimPhyzHelpers::getNameFromConstraint(params->constr_type); 
	
}

void ExConstraintDict::getDefaultNames(TMap<EnExConstraintParamNames, FString>* trg)
{
	trg->FindOrAdd(EnExConstraintParamNames::parent_pivot) = TEXT("Parent Pivot"); 
	trg->FindOrAdd(EnExConstraintParamNames::target_pivot) = TEXT("Target Pivot");  
	trg->FindOrAdd(EnExConstraintParamNames::parent_axis) = TEXT("Parent Axis");  
	trg->FindOrAdd(EnExConstraintParamNames::target_axis) = TEXT("Target Axis"); 
	trg->FindOrAdd(EnExConstraintParamNames::dump_ang) = TEXT("Dumping Angular");  
	trg->FindOrAdd(EnExConstraintParamNames::dump_lin) = TEXT("Dumping Linear");  
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_ang) = TEXT("Angular Lower limit");  
	trg->FindOrAdd(EnExConstraintParamNames::upp_lim_ang) = TEXT("Angular Upper Limit");  
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_lin) = TEXT("Linear Lower Limit");  
	trg->FindOrAdd(EnExConstraintParamNames::upp_lim_lin) = TEXT("Linear Upper Limit");  
	trg->FindOrAdd(EnExConstraintParamNames::stiff_ang) = TEXT("Angular Stiffness");  
	trg->FindOrAdd(EnExConstraintParamNames::stiff_lin) = TEXT("Linear Stiffness");  
	
	trg->FindOrAdd(EnExConstraintParamNames::constraint_name) =  TEXT("Constraint Name");  
	trg->FindOrAdd(EnExConstraintParamNames::parent_name) = TEXT("Parent Name");   
	trg->FindOrAdd(EnExConstraintParamNames::target_name) = TEXT("Target Name");   

	trg->FindOrAdd(EnExConstraintParamNames::tau) = TEXT("Tau");  
	trg->FindOrAdd(EnExConstraintParamNames::impulse_clamp) = TEXT("Impulse Clamp");  
	trg->FindOrAdd(EnExConstraintParamNames::lower_limit) = TEXT("Lower Limit");  
	trg->FindOrAdd(EnExConstraintParamNames::upper_limit) = TEXT("Upper Limit");  

	trg->FindOrAdd(EnExConstraintParamNames::enables_spring) =  TEXT("Enabled Spring"); 

	trg->FindOrAdd(EnExConstraintParamNames::constraint_t) =  TEXT("Constraint Type"); 
}

void ExConstraintDict::getNameValuePairs(TMap<EnExConstraintParamNames, FString>* names,
                                         TMap<EnExConstraintParamNames, FString>* values, TMap<FString, FString>* trg)
{
	for(auto & n : *names)
	{
		FString name = n.Value;
		EnExConstraintParamNames c = n.Key;
		FString * value = values->Find(c);
		trg->FindOrAdd(name) = *value;
	}
}

void ExConstraintDict::fromNameValuePairsToParams(TMap<FString, FString>* src, FExConstraintParams* trg)
{
	TMap<EnExConstraintParamNames, FString> names;
	getDefaultNames(&names);
	for (auto name : names)
	{
		const auto str = src->FindRef(name.Value);
		ExConvert::updateParams(trg, name.Key, str);
	}
}

void ExConstraintDict::getInitValues(TMap<EnExConstraintParamNames, TArray<FString>>* trg, FExConstraintParams* params)
{
	trg->FindOrAdd(EnExConstraintParamNames::parent_axis) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->axis_p));
	trg->FindOrAdd(EnExConstraintParamNames::target_axis) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->axis_t));
	trg->FindOrAdd(EnExConstraintParamNames::dump_ang) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->dump_ang));
	trg->FindOrAdd(EnExConstraintParamNames::dump_lin) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->dump_lin));
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_ang) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->low_lim_ang));
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_lin) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->low_lim_lin));
	trg->FindOrAdd(EnExConstraintParamNames::parent_pivot) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->pivot_p));
	trg->FindOrAdd(EnExConstraintParamNames::target_pivot) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->pivot_t));
	trg->FindOrAdd(EnExConstraintParamNames::stiff_ang) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->stiff_ang));
	trg->FindOrAdd(EnExConstraintParamNames::stiff_lin) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->stiff_lin));
	trg->FindOrAdd(EnExConstraintParamNames::upp_lim_ang) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->upp_lim_ang));
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_lin) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->upp_lim_lin));


	
	trg->FindOrAdd(EnExConstraintParamNames::constraint_name) = ExConvert::getArrayString(params->name_constraint); 
	trg->FindOrAdd(EnExConstraintParamNames::parent_name) = ExConvert::getArrayString(params->name_p); 
	trg->FindOrAdd(EnExConstraintParamNames::target_name) = ExConvert::getArrayString(params->name_t); 

	trg->FindOrAdd(EnExConstraintParamNames::impulse_clamp) = ExConvert::getArrayString(ExConvert::getStrFromFloat(params->impulse_clamp)); 
	trg->FindOrAdd(EnExConstraintParamNames::lower_limit) = ExConvert::getArrayString(ExConvert::getStrFromFloat(params->lower_limit)); 
	trg->FindOrAdd(EnExConstraintParamNames::tau) = ExConvert::getArrayString(ExConvert::getStrFromFloat(params->tau)); 
	trg->FindOrAdd(EnExConstraintParamNames::upper_limit) = ExConvert::getArrayString(ExConvert::getStrFromFloat(params->upper_limit)); 


	trg->FindOrAdd(EnExConstraintParamNames::enables_spring) = ExConvert::getArrayString(ExConvert::getBoolStrFromInt(params->enables_spring, 6));

	trg->FindOrAdd(EnExConstraintParamNames::constraint_t) = ExSimPhyzHelpers::getConstraintsList(params->constr_type); 
	
}

void ExConstraintDict::getEditTypeCreate(TMap<EnExConstraintParamNames, EnExParamEdit>* trg)
{
	trg->FindOrAdd(EnExConstraintParamNames::parent_pivot) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::target_pivot) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::parent_axis) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::target_axis) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::dump_ang) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::dump_lin) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_ang) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::upp_lim_ang) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::low_lim_lin) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::upp_lim_lin) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::stiff_ang) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::stiff_lin) = EnExParamEdit::EDITABLE; 
	
	trg->FindOrAdd(EnExConstraintParamNames::constraint_name) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::parent_name) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::target_name) = EnExParamEdit::EDITABLE; 

	trg->FindOrAdd(EnExConstraintParamNames::tau) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::impulse_clamp) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::lower_limit) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExConstraintParamNames::upper_limit) = EnExParamEdit::EDITABLE; 

	trg->FindOrAdd(EnExConstraintParamNames::enables_spring) =  EnExParamEdit::EDITABLE; 

	trg->FindOrAdd(EnExConstraintParamNames::constraint_t) = EnExParamEdit::SELECTABLE;
}

