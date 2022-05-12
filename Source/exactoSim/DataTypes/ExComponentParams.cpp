#include "ExComponentParams.h"

#include "exactoSim/Utils/ExConvert.h"

void ExComponentDict::updateValues(TMap<EnExComponentParamNames, FString>* trg, FExComponentParams* params)
{
	trg->FindOrAdd(EnExComponentParamNames::A_POSITION) = ExConvert::getStrFromVec(params->Position); 
	trg->FindOrAdd(EnExComponentParamNames::C_NAME) = params->Name; 
	trg->FindOrAdd(EnExComponentParamNames::C_PATH) = params->Path; 
	trg->FindOrAdd(EnExComponentParamNames::D_MASS) = ExConvert::getStrFromFloat(params->Mass); 
	trg->FindOrAdd(EnExComponentParamNames::B_ROTATION) = ExConvert::getStrFromRot(params->Rotation); 
}

void ExComponentDict::getDefaultNames(TMap<EnExComponentParamNames, FString>* trg)
{
	trg->FindOrAdd(EnExComponentParamNames::D_MASS) = TEXT("Mass"); 
	trg->FindOrAdd(EnExComponentParamNames::C_NAME) = TEXT("Component Name"); 
	trg->FindOrAdd(EnExComponentParamNames::C_PATH) = TEXT("Path to Blueprint"); 
	trg->FindOrAdd(EnExComponentParamNames::A_POSITION) = TEXT("Init Local Position"); 
	trg->FindOrAdd(EnExComponentParamNames::B_ROTATION) = TEXT("Init Local Rotation"); 
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

