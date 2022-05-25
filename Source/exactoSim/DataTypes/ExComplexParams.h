#pragma once
#include "ExCommonParams.h"

#include "ExComplexParams.generated.h"

enum class EnExComplexParamNames : int
{
	AA_VECTOR_START = 0,
	BA_ROTATOR_START,
	CA_STRING_START,
	C_NAME ,
	C_BASIS_NAME,
	DA_FLOAT_START,
	EA_STRING_ARRAY_START,
	ZZ_OPT_END,
};

USTRUCT()
struct EXACTOSIM_API FExComplexParams : public FExCommonParams
{
	GENERATED_BODY()
	FString Name;
	FString BasisName;
	TArray<FString> ComponentNames;
	FExComplexParams()
	{
		setType(EnExParamTypes::COMPLEX);
	}
	bool update(EnExComplexParamNames type, FString val);
	bool update(EnExComplexParamNames type, FVector val);
	bool update(EnExComplexParamNames type, FRotator val);
	bool update(EnExComplexParamNames type, float val);
	bool update(EnExComplexParamNames type, int32 val);
	bool copy(EnExComplexParamNames type, FString * trg);
	bool copy(EnExComplexParamNames type, FVector * trg);
	bool copy(EnExComplexParamNames type, FRotator * trg);
	bool copy(EnExComplexParamNames type, float * trg);
	bool copy(EnExComplexParamNames type, int32 * trg);
};


class EXACTOSIM_API ExComplexDict
{
public:
	static void getInitValues(TMap<EnExComplexParamNames, TArray<FString>>* trg, FExComplexParams* params);
	static void updateValues(TMap<EnExComplexParamNames, FString>* trg, FExComplexParams* params);
	static void getDefaultNames(TMap<EnExComplexParamNames, FString>* trg);
	static void getNameValuePairs(TMap<EnExComplexParamNames, FString>* names,
	                              TMap<EnExComplexParamNames, FString>* values, TMap<FString, FString>* trg);
	static void fromNameValuePairsToParams(TMap<FString, FString> * src, FExComplexParams * trg);
	static void getEditTypeCreate(TMap<EnExComplexParamNames, EnExParamEdit>* trg);
	static void getEditTypeUpdate(TMap<EnExComplexParamNames, EnExParamEdit>* trg);
};

