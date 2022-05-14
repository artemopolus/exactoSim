#include "ExComplexParams.h"

#include "exactoSim/Utils/ExConvert.h"

void ExComplexDict::updateValues(TMap<EnExComplexParamNames, FString>* trg, FExComplexParams* params)
{
	trg->FindOrAdd(EnExComplexParamNames::C_NAME) = params->Name; 
	trg->FindOrAdd(EnExComplexParamNames::C_BASIS_NAME) = params->BasisName; 
}

void ExComplexDict::getDefaultNames(TMap<EnExComplexParamNames, FString>* trg)
{
	trg->FindOrAdd(EnExComplexParamNames::C_NAME) = TEXT("Name"); 
	trg->FindOrAdd(EnExComplexParamNames::C_BASIS_NAME) = TEXT("BasisName"); 
}

void ExComplexDict::getNameValuePairs(TMap<EnExComplexParamNames, FString>* names,
	TMap<EnExComplexParamNames, FString>* values, TMap<FString, FString>* trg)
{
	for(auto & n : *names)
	{
		FString name = n.Value;
		EnExComplexParamNames c = n.Key;
		FString * value = values->Find(c);
		trg->FindOrAdd(name) = *value;
	}
}

void ExComplexDict::fromNameValuePairsToParams(TMap<FString, FString>* src, FExComplexParams* trg)
{
	TMap<EnExComplexParamNames, FString> names;
	getDefaultNames(&names);
	for (auto name : names)
	{
		const auto str = src->FindRef(name.Value);
		ExConvert::updateParams(trg, name.Key, str);
	}
}
