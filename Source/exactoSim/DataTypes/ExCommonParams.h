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
	bool NeedToCreate = false;
	bool NeedToDelete = false;
	void * Owner = nullptr;
public:
	void setType(const EnExParamTypes type){ParamType = type;}
	EnExParamTypes getType() const {return ParamType;}

	void markToCreate( )
	{
		NeedToCreate = true;
		NeedToDelete = false;
	}
	bool isMarkedToCreate() const {return NeedToCreate;}

	void markToDelete()
	{
		NeedToDelete = true;
		NeedToCreate = false;
	}
	void resetMarkers()
	{
		NeedToCreate = false;
		NeedToDelete = false;
	}
	bool isMarkedToDelete()const{return  NeedToDelete;}

	void setOwner(void * trg){Owner = trg;}
	void * getOwner()const{return Owner;}
	
};

