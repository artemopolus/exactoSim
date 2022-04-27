#pragma once
#include "ExSimPhyzHelpers.h"
#include "FExConstraintParams.h"

class EXACTOSIM_API ExSimComplex;
class EXACTOSIM_API ExSimComponent;

class EXACTOSIM_API ExSimConstraintPair
{
	FString Name;
	ExSimComponent* Parent = nullptr;
	btTypedConstraint* Constraint = nullptr;
	ExSimPhyzHelpers::Constraint Type = ExSimPhyzHelpers::Constraint::NONE;
	FExConstraintParams* Params = nullptr;
public:
	FString getName(){return Name;}
	FExConstraintParams * getParams(){return Params;}
	btTypedConstraint * getConstraint(){return Constraint;}
	ExSimPhyzHelpers::Constraint getType(){return Type;}
	ExSimComponent * getParent(){return Parent;}

	void setName(FString n){Name = n;}
	void setConstraint(btTypedConstraint * c){Constraint = c;}
	void setParams(FExConstraintParams * p){Params = p;}
	void setType(ExSimPhyzHelpers::Constraint t){Type = t;}
	void setParent(ExSimComponent * p){Parent = p;}
};

class EXACTOSIM_API ExSimComponent
{
	FString Name;
	FString Path;
	AActor* Target;
	btRigidBody* Body;
	TArray<ExSimConstraintPair*> Constraints;
	ExSimComplex* Basis;
public:
	bool getConstraintNames(TArray<FString>* names);
	bool addConstraint(FString name);
	bool removeConstraint(FString name);
	
	FString getName(){return  Name;}
	FString getPath(){return Path;}
	AActor * getTarget(){return Target;}
	btRigidBody * getBody(){return Body;}
	TArray<ExSimConstraintPair*> * getConstraints(){return &Constraints;}
	ExSimComplex * getBasis(){return Basis;}
	
	void setName(FString name){Name = name;}
	void setPath(FString path){Path = path;}
	void setTarget(AActor * target){Target = target;}
	void setBody(btRigidBody * body){Body = body;}
	void setBasis(ExSimComplex * basis){Basis = basis;}
};

class ExSimComplex
{
	FString Name;
	ExSimComponent* Basis = nullptr;
	TArray<ExSimComponent*> Components;
public:
	~ExSimComplex();
	void setName(FString name){Name = name;}
	void setBasis(ExSimComponent * basis){Basis = basis;}

	FString getName(){return Name;}
	ExSimComponent * getBasis(){return Basis;}
	TArray<ExSimComponent*> * getComponents(){return &Components;}
};
