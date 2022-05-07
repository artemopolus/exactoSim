#pragma once
#include "FExConstraintParams.h"

#include "ExComponentParams.generated.h"

USTRUCT()
struct EXACTOSIM_API FExComponentParams
{
	GENERATED_BODY()
	
	FString Name;
	FString Path;
	float Mass;
	FVector Position;
	FRotator Rotation;
	FExComponentParams()
	{
		Name = Path = "Default";
		Mass = 0.f;
		Position = FVector::ZeroVector;
		Rotation = FRotator::ZeroRotator;
	}
	FExComponentParams(FString name, FString path, float mass, FVector pos, FRotator rot)
	{
		Name = name;
		Path = path;
		Mass = mass;
		Position = pos;
		Rotation = rot;
	}
};
enum class EnExComponentParamNames : int
{
	NAME = 0,
	PATH,
	MASS,
	POSITION,
	ROTATION
};
class EXACTOSIM_API ExComponentDict
{
public:
	static void updateValues(TMap<EnExComponentParamNames, FString>* trg, FExComponentParams* params);
	static void getDefaultNames(TMap<EnExComponentParamNames, FString>* trg);
	static void getNameValuePairs(TMap<EnExComponentParamNames, FString>* names,
	                              TMap<EnExComponentParamNames, FString>* values, TMap<FString, FString>* trg);
};
