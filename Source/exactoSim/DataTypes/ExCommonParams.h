#pragma once

#include "ExCommonParams.generated.h"

enum class  EnExParamTypes : int
{
	CONSTRAINT = 0,
	COMPONENT,
	COMPLEX
};

USTRUCT()
struct EXACTOSIM_API FExCommonParams
{
	GENERATED_BODY()
private:
	EnExParamTypes ParamType = EnExParamTypes::CONSTRAINT;
public:
	void setType(const EnExParamTypes type){ParamType = type;}
	EnExParamTypes getType() const {return ParamType;}
};

