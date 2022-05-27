#pragma once
#include "ExComplexParams.h"
#include "ExComponentParams.h"
#include "ExSimObject.h"
#include "ExSimPhyzHelpers.h"
#include "FExConstraintParams.h"





class EXACTOSIM_API ExSimComplex;
class EXACTOSIM_API ExSimComponent;




class EXACTOSIM_API ExSimConstraintPair : public ExSimObject
{
	FString Name;
	ExSimComponent* Parent = nullptr;
	btTypedConstraint* Constraint = nullptr;
	ExSimPhyzHelpers::Constraint Type = ExSimPhyzHelpers::Constraint::NONE;
	FExConstraintParams* Params = nullptr;
public:
	ExSimConstraintPair()
	{
		setObjType(EnExParamTypes::CONSTRAINT);
		Params = new FExConstraintParams(); //todo: have not to be there
		Params->setOwner(this);
	}
	ExSimConstraintPair(ExSimComponent * p, FExConstraintParams * params):
		Name(params->name_constraint), Parent(p), Type(params->constr_type), Params(params)
	{
		params->setOwner(this);
		setObjType(EnExParamTypes::CONSTRAINT);
	}
	~ExSimConstraintPair(){}
	virtual FString getName() override {return Name;}
	FExConstraintParams * getParams() const {return Params;}
	btTypedConstraint * getConstraint() const {return Constraint;}
	ExSimPhyzHelpers::Constraint getType() const {return Type;}
	ExSimComponent * getParent() const {return Parent;}

	void setName(FString n){Name = n;}
	void setConstraint(btTypedConstraint * c){Constraint = c;}
	void setParams(FExConstraintParams * p){Params = p;}
	void setType(ExSimPhyzHelpers::Constraint t){Type = t;}
	void setParent(ExSimComponent * p){Parent = p;}
	virtual void update() override;
};

class EXACTOSIM_API ExSimComponent : public ExSimObject
{
	FString Name;
	FString Path;
	AActor* Target = nullptr;
	btRigidBody* Body = nullptr;
	TArray<ExSimConstraintPair*> Constraints;
	ExSimComplex* Basis = nullptr;
	FExComponentParams * Params = nullptr;
public:
	ExSimComponent()
	{
		setObjType(EnExParamTypes::COMPONENT);
		Params->setOwner(this);
	}

	ExSimComponent(FExCommonParams* params)
	{
		setObjType(EnExParamTypes::COMPONENT);
		Params = static_cast<FExComponentParams*>(params);
		Params->setOwner(this);
	}

	ExSimComponent(FExComponentParams* params) : Params(params)
	{
		setObjType(EnExParamTypes::COMPONENT);
		Params->setOwner(this);
	}
	bool getConstraintNames(TArray<FString>* names);

	virtual FString getName() override {return  Name;}
	FString getPath(){return Path;}
	AActor * getTarget(){return Target;}
	btRigidBody * getBody(){return Body;}
	TArray<ExSimConstraintPair*> * getConstraints(){return &Constraints;}
	ExSimComplex * getBasis(){return Basis;}
	FExComponentParams * getParams(){return Params;}
	FString getComplexName();
	
	void setName(FString name){Name = name;}
	void setPath(FString path){Path = path;}
	void setTarget(AActor * target){Target = target;}
	void setBody(btRigidBody * body){Body = body;}
	
	void setBasis(ExSimComplex * basis){Basis = basis;}
	void setBasis(TArray<ExSimComplex*> * list);
	
	void setParams(FExComponentParams * params){Params = params;}

	void addConstraint(ExSimConstraintPair * constr);
	void removeConstraint(ExSimConstraintPair * constr);
	
	virtual void update() override;
};

class ExSimComplex : public ExSimObject
{
	FString Name;
	FString BasisName;
	ExSimComponent* Basis = nullptr;
	TArray<ExSimComponent*> Components;
	FExComplexParams * Params;
public:
	ExSimComplex()
	{
		setObjType(EnExParamTypes::COMPLEX);
	}
	ExSimComplex(FExComplexParams * params) : Params(params)
	{
		Name = params->Name;
		BasisName = params->BasisName;
		setObjType(EnExParamTypes::COMPLEX);
	}

	virtual ~ExSimComplex() override;
	void setName(FString name){Name = name;}
	void setBasisName(FString name){BasisName = name;}
	void setBasis(ExSimComponent * basis)
	{
		Basis = basis;
	}
	void setParams(FExComplexParams * params){Params = params;}

	virtual FString getName() override {return Name;}
	FString getBasisName(){return BasisName;}
	ExSimComponent * getBasis(){return Basis;}
	TArray<ExSimComponent*> * getComponentsList(){return &Components;}

	void addComponent(ExSimComponent * component);
	void removeComponent(ExSimComponent * component);

	void addComponents(TArray<ExSimComponent*> * targets);
	void moveComponents(ExSimComplex * trg);

	ExSimComponent * getComponent(FString name);
	FExComplexParams * getParams() const {return Params;}
	virtual void update() override;
};


