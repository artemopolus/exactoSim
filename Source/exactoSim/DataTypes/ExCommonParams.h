#pragma once
#include "ExSimObject.h"
#include "ExSimPhyzHelpers.h"

#include "ExCommonParams.generated.h"

enum class  EnExParamEdit : int32
{
	EDITABLE = 0,
	SELECTABLE,
	CLICKABLE	
};

USTRUCT()
struct EXACTOSIM_API FExCommonParams
{
	GENERATED_BODY()

public:
	enum class Action : int32
	{
		NOTHING = 0,
		CREATE,
		HIDE,
		SHOW,
		DELETE
	};
	void setType(const EnExParamTypes type){ParamType = type;}
	EnExParamTypes getType() const {return ParamType;}


	void markToCreate( ){	Need = Action::CREATE;	}
	void markToDelete() {	Need = Action::DELETE;	}
	void markToHide()	{	Need = Action::HIDE;	}
	void markToShow()	{	Need = Action::SHOW;	}
	void resetMarkers()	{	Need = Action::NOTHING;	}
	
	bool isMarkedToCreate() const	{return Need == Action::CREATE;}
	bool isMarkedToDelete() const	{return Need == Action::DELETE;}
	bool isMarkedToHide()	const	{return Need == Action::HIDE;}
	bool isMarkedToShow()	const	{return Need == Action::SHOW;}

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
private:
	EnExParamTypes ParamType = EnExParamTypes::CONSTRAINT;
	Action Need = Action::NOTHING;
	ExSimObject * Owner = nullptr;
	ExSimObject * DataPointer = nullptr;	
};



