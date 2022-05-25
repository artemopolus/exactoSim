#pragma once
#include "ExObjectFactory.h"
#include "ExParamFactory.h"
#include "ExSimComponent.h"

class ExFactoryOperator
{
	ExConstraintParamFactory ConstraintParam;
	ExComponentParamFactory ComponentParam;
	ExComplexParamFactory ComplexParam;

	ExConstraintPairFactory ConstraintPair;
	ExComponentFactory Component;
	ExComplexFactory Complex;
	
public:
	ExFactoryOperator();

	FExConstraintParams* createConstraintParams();
	FExConstraintParams* createConstraintParams(TMap<FString, FString>* names);
	
	FExComponentParams * createComponentParams();
	FExComponentParams * createComponentParams(FExComponentParams * to_copy);
	FExComponentParams * createComponentParams(TMap<FString, FString> * names);

	FExComplexParams * createComplexParams();
	FExComplexParams * createComplexParams(TMap<FString, FString> * names);

	ExSimConstraintPair * createConstraintPair( FExCommonParams * params);
	ExSimConstraintPair * createConstraintPair( FExConstraintParams * params);
	void deleteConstraintPair( ExSimConstraintPair * params);

	ExSimComponent * createComponent(FExComponentParams * params);
	void deleteComponent(ExSimComponent * params);
	void deleteComplex(ExSimComplex * params);

	ExSimComplex * createComplex(FExComplexParams * params);
};
