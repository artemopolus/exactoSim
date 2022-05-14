#pragma once
#include "ExCommonParams.h"

#include "ExComplexParams.generated.h"

USTRUCT()
struct EXACTOSIM_API FExComplexParams : public FExCommonParams
{
	GENERATED_BODY()
	FString Name;
	FString BasisName;
	FExComplexParams()
	{
		setType(EnExParamTypes::COMPLEX);
	}
	
};
enum class EnExComplexParamNames : int
{
	AA_VECTOR_START = 0,
	BA_ROTATOR_START,
	CA_STRING_START,
	C_NAME ,
	C_BASIS_NAME,
	DA_FLOAT_START,
	ZZ_OPT_END,
};

class EXACTOSIM_API ExComplexDict
{
public:
	static void updateValues(TMap<EnExComplexParamNames, FString>* trg, FExComplexParams* params);
	static void getDefaultNames(TMap<EnExComplexParamNames, FString>* trg);
	static void getNameValuePairs(TMap<EnExComplexParamNames, FString>* names,
	                              TMap<EnExComplexParamNames, FString>* values, TMap<FString, FString>* trg);
	static void fromNameValuePairsToParams(TMap<FString, FString> * src, FExComplexParams * trg);
};

