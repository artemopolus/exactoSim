#pragma once
#include "ExSimObject.h"
#include "ExSimPhyzHelpers.h"

#include "ExCommonParams.generated.h"

enum class  EnExParamEdit : int
{
	EDITABLE = 0,
	SELECTABLE,
	CLICKABLE	
};

USTRUCT()
struct EXACTOSIM_API FExCommonParams
{
	GENERATED_BODY()
private:
	EnExParamTypes ParamType = EnExParamTypes::CONSTRAINT;
	bool NeedToCreate = false;
	bool NeedToDelete = false;
	ExSimObject * Owner = nullptr;
	ExSimObject * DataPointer = nullptr;
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

	void setOwner(ExSimObject * trg){Owner = trg;}
	ExSimObject * getOwner()const{return Owner;}

	void setDataPointer(ExSimObject * data){DataPointer = data;}
	ExSimObject * getDataPointer()
	{
		if(DataPointer == nullptr)
			return nullptr;
		ExSimObject * data = DataPointer;
		DataPointer = nullptr;
		return data;
	}

	bool isConstraint() const {return ParamType == EnExParamTypes::CONSTRAINT;}
	bool isComponent() const {return ParamType == EnExParamTypes::COMPONENT;}
	bool isComplex() const {return ParamType == EnExParamTypes::COMPLEX;}
	
};



