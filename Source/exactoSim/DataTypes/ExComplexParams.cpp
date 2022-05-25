#include "ExComplexParams.h"

#include "exactoSim/Utils/ExConvert.h"

bool FExComplexParams::update(EnExComplexParamNames type, FString val)
{
	if (type == EnExComplexParamNames::C_NAME)
		Name = val;
	else if (type == EnExComplexParamNames::C_BASIS_NAME)
		BasisName = val;
	else return false;
	return true;
}

bool FExComplexParams::update(EnExComplexParamNames type, FVector val)
{
	return false;
}

bool FExComplexParams::update(EnExComplexParamNames type, FRotator val)
{
	return false;
}

bool FExComplexParams::update(EnExComplexParamNames type, float val)
{
	return false;
}

bool FExComplexParams::update(EnExComplexParamNames type, int32 val)
{
	return false;
}

bool FExComplexParams::copy(EnExComplexParamNames type, FString* trg)
{
	if (type == EnExComplexParamNames::C_NAME)
		*trg = Name;
	else if (type == EnExComplexParamNames::C_BASIS_NAME)
		*trg = BasisName;
	else return false;
	return true;
}

bool FExComplexParams::copy(EnExComplexParamNames type, FVector* trg)
{
	return false;
}

bool FExComplexParams::copy(EnExComplexParamNames type, FRotator* trg)
{
	return false;
}

bool FExComplexParams::copy(EnExComplexParamNames type, float* trg)
{
	return false;
}

bool FExComplexParams::copy(EnExComplexParamNames type, int32* trg)
{
	return false;
}

void ExComplexDict::getInitValues(TMap<EnExComplexParamNames, TArray<FString>>* trg, FExComplexParams* params)
{
	trg->FindOrAdd(EnExComplexParamNames::C_NAME) = ExConvert::getArrayString(params->Name); 
	trg->FindOrAdd(EnExComplexParamNames::C_BASIS_NAME) = ExConvert::getArrayString(params->BasisName); 
}

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

void ExComplexDict::getEditTypeCreate(TMap<EnExComplexParamNames, EnExParamEdit>* trg)
{
	trg->FindOrAdd(EnExComplexParamNames::C_NAME) = EnExParamEdit::EDITABLE; 
	trg->FindOrAdd(EnExComplexParamNames::C_BASIS_NAME) = EnExParamEdit::EDITABLE; 
}

void ExComplexDict::getEditTypeUpdate(TMap<EnExComplexParamNames, EnExParamEdit>* trg)
{
}
