#include "ExComponentParams.h"

#include "exactoSim/Utils/ExConvert.h"

void ExComponentDict::updateValues(TMap<EnExComponentParamNames, FString>* trg, FExComponentParams* params)
{
	trg->FindOrAdd(EnExComponentParamNames::POSITION) = ExConvert::getStrFromVec(params->Position); 
	trg->FindOrAdd(EnExComponentParamNames::NAME) = params->Name; 
	trg->FindOrAdd(EnExComponentParamNames::PATH) = params->Path; 
	trg->FindOrAdd(EnExComponentParamNames::MASS) = ExConvert::getStrFromFloat(params->Mass); 
	trg->FindOrAdd(EnExComponentParamNames::ROTATION) = ExConvert::getStrFromRot(params->Rotation); 
}

void ExComponentDict::getDefaultNames(TMap<EnExComponentParamNames, FString>* trg)
{
	trg->FindOrAdd(EnExComponentParamNames::MASS) = TEXT("Mass"); 
	trg->FindOrAdd(EnExComponentParamNames::NAME) = TEXT("Component Name"); 
	trg->FindOrAdd(EnExComponentParamNames::PATH) = TEXT("Path to Blueprint"); 
	trg->FindOrAdd(EnExComponentParamNames::POSITION) = TEXT("Init Local Position"); 
	trg->FindOrAdd(EnExComponentParamNames::ROTATION) = TEXT("Init Local Rotation"); 
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
