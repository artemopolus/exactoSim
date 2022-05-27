#pragma once
#include "ExSimPhyzHelpers.h"

class EXACTOSIM_API ExSimObject
{
	EnExParamTypes ObjType = EnExParamTypes::CONSTRAINT;
public:
	virtual ~ExSimObject() = default;
	EnExParamTypes getObjType() const {return  ObjType;}
	void setObjType(EnExParamTypes type){ObjType = type;}
	bool isConstraint() const{return ObjType == EnExParamTypes::CONSTRAINT;}
	bool isComponent() const{return ObjType == EnExParamTypes::COMPONENT;}
	bool isComplex() const{return ObjType == EnExParamTypes::COMPLEX;}
	virtual void update() = 0;
	virtual FString getName() = 0;
};
