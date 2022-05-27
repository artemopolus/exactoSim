
#pragma once

#include "ExComplexParams.h"
#include "ExComponentParams.h"
#include "FExConstraintParams.h"

class EXACTOSIM_API ExParamFactory
{
public:
	virtual ~ExParamFactory() = default;
	virtual FExCommonParams * factoryMethod() = 0;
};

class EXACTOSIM_API ExConstraintParamFactory : public ExParamFactory
{
public:
	virtual FExCommonParams* factoryMethod() override
	{
		return new FExConstraintParams();
	}
	FExConstraintParams * factoryMethod(TMap<FString, FString> * names)
	{
		FExConstraintParams * params = new FExConstraintParams();
		ExConstraintDict::fromNameValuePairsToParams( names, params);
		return params;	
	}
};

class EXACTOSIM_API ExComponentParamFactory : public ExParamFactory
{
	FVector StartPosition = FVector::ZeroVector;
	FRotator StartRotation = FRotator::ZeroRotator;
	FString InitName = TEXT("Component");
	FString InitPath = TEXT("Class'/Game/Blueprint/Scene/BP_ExSmplBox_Simple.BP_ExSmplBox_Simple_C'");
public:
	void setStartPosition(const FVector pos) {StartPosition = pos;}
	virtual FExCommonParams* factoryMethod() override
	{
		static int32 index = 0;
		const FString name = InitName + TEXT("_") + FString::FromInt(index++);
		return new FExComponentParams(name, InitPath, StartPosition, StartRotation);
	}
	FExComponentParams * factoryMethod(TMap<FString, FString> * names)
	{
		FExComponentParams * params = static_cast<FExComponentParams*>(factoryMethod());
		ExComponentDict::fromNameValuePairsToParams( names, params);
		return params;	
	}
};

class EXACTOSIM_API ExComplexParamFactory : public ExParamFactory
{
public:
	virtual FExCommonParams* factoryMethod() override
	{
		return new FExComplexParams();
	}
	FExComplexParams * factoryMethod(TMap<FString, FString> * names)
	{
		FExComplexParams * params = new FExComplexParams();
		ExComplexDict::fromNameValuePairsToParams( names, params);
		return params;	
	}
};

