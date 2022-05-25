#include "ExComponentParams.h"

#include "exactoSim/Utils/ExConvert.h"

void ExComponentDict::updateValues(TMap<EnExComponentParamNames, FString>* trg, FExComponentParams* params)
{
	trg->FindOrAdd(EnExComponentParamNames::A_POSITION) = ExConvert::getStrFromVec(params->Position); 
	trg->FindOrAdd(EnExComponentParamNames::B_ROTATION) = ExConvert::getStrFromRot(params->Rotation); 
	trg->FindOrAdd(EnExComponentParamNames::C_NAME) = params->Name; 
	trg->FindOrAdd(EnExComponentParamNames::C_PATH) = params->Path; 
	trg->FindOrAdd(EnExComponentParamNames::D_MASS) = ExConvert::getStrFromFloat(params->Mass); 
	trg->FindOrAdd(EnExComponentParamNames::E_IS_PHYZ) = FString::FromInt(params->IsOnPhysics); 
}

void ExComponentDict::getInitValues(TMap<EnExComponentParamNames, TArray<FString>>* trg, FExComponentParams* params)
{
	trg->FindOrAdd(EnExComponentParamNames::A_POSITION) = ExConvert::getArrayString(ExConvert::getStrFromVec(params->Position));
	trg->FindOrAdd(EnExComponentParamNames::B_ROTATION) = ExConvert::getArrayString(ExConvert::getStrFromRot(params->Rotation)); 
	trg->FindOrAdd(EnExComponentParamNames::C_NAME) = ExConvert::getArrayString(params->Name); 
	trg->FindOrAdd(EnExComponentParamNames::C_PATH) = ExConvert::getArrayString(params->Path); 
	trg->FindOrAdd(EnExComponentParamNames::D_MASS) = ExConvert::getArrayString(ExConvert::getStrFromFloat(params->Mass)); 
	trg->FindOrAdd(EnExComponentParamNames::E_IS_PHYZ) = ExConvert::getArrayBool(params->IsOnPhysics); 	
}

void ExComponentDict::getDefaultNames(TMap<EnExComponentParamNames, FString>* trg)
{
	trg->FindOrAdd(EnExComponentParamNames::A_POSITION) = TEXT("Init Local Position"); 
	trg->FindOrAdd(EnExComponentParamNames::B_ROTATION) = TEXT("Init Local Rotation"); 
	trg->FindOrAdd(EnExComponentParamNames::C_NAME) = TEXT("Component Name"); 
	trg->FindOrAdd(EnExComponentParamNames::C_PATH) = TEXT("Path to Blueprint");
	trg->FindOrAdd(EnExComponentParamNames::D_MASS) = TEXT("Mass");
	trg->FindOrAdd(EnExComponentParamNames::E_IS_PHYZ) = TEXT("IsPhysical");
	
}

void ExComponentDict::getEditTypeCreate(TMap<EnExComponentParamNames, EnExParamEdit>* trg)
{
	trg->FindOrAdd(EnExComponentParamNames::A_POSITION) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExComponentParamNames::B_ROTATION) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExComponentParamNames::C_NAME) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExComponentParamNames::C_PATH) = EnExParamEdit::CLICKABLE;
	trg->FindOrAdd(EnExComponentParamNames::D_MASS) = EnExParamEdit::EDITABLE;
	trg->FindOrAdd(EnExComponentParamNames::E_IS_PHYZ) = EnExParamEdit::SELECTABLE;
}

void ExComponentDict::getEditTypeUpdate(TMap<EnExComponentParamNames, EnExParamEdit>* trg)
{
}

void ExComponentDict::getNameValuePairs(TMap<EnExComponentParamNames, FString>* names,
                                        TMap<EnExComponentParamNames, FString>* values, TMap<FString, FString>* trg)
{
	for(auto & n : *names)
	{
		FString name = n.Value;
		EnExComponentParamNames c = n.Key;
		FString * value = values->Find(c);
		trg->FindOrAdd(name) = *value;
	}
}

void ExComponentDict::fromNameValuePairsToParams(TMap<FString, FString>* src, FExComponentParams* trg)
{
	TMap<EnExComponentParamNames, FString> names;
	getDefaultNames(&names);
	for (auto name : names)
	{
		const auto str = src->FindRef(name.Value);
		ExConvert::updateParams(trg, name.Key, str);
	}
}

