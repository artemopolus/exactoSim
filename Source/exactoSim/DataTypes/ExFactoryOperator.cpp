#include "ExFactoryOperator.h"

ExFactoryOperator::ExFactoryOperator()
{
	ComponentParam.setStartPosition(FVector(0,0,50));
}

FExConstraintParams* ExFactoryOperator::createConstraintParams()
{
	return static_cast<FExConstraintParams*>(ConstraintParam.factoryMethod());
}

FExConstraintParams* ExFactoryOperator::createConstraintParams(TMap<FString, FString>* names)
{
	return ConstraintParam.factoryMethod(names);
}

FExComponentParams* ExFactoryOperator::createComponentParams()
{
	return static_cast<FExComponentParams*>(ComponentParam.factoryMethod());
}

FExComponentParams* ExFactoryOperator::createComponentParams(FExComponentParams* to_copy)
{
	FExComponentParams * copy = static_cast<FExComponentParams*>(ComponentParam.factoryMethod());
	*copy = *to_copy;
	return copy;
}

FExComponentParams* ExFactoryOperator::createComponentParams(TMap<FString, FString>* names)
{
	return  ComponentParam.factoryMethod(names);
}

FExComplexParams* ExFactoryOperator::createComplexParams()
{
	return static_cast<FExComplexParams*>(ComplexParam.factoryMethod());
}

FExComplexParams* ExFactoryOperator::createComplexParams(TMap<FString, FString>* names)
{
	return  ComplexParam.factoryMethod(names);
}

ExSimConstraintPair* ExFactoryOperator::createConstraintPair(FExCommonParams* params)
{
	return static_cast<ExSimConstraintPair*>(ConstraintPair.factoryMethod(params));
}

ExSimConstraintPair* ExFactoryOperator::createConstraintPair(FExConstraintParams* params)
{
	return static_cast<ExSimConstraintPair*>(ConstraintPair.factoryMethod(params));
}

void ExFactoryOperator::deleteConstraintPair( ExSimConstraintPair * params)
{
	delete params;
}

ExSimComponent* ExFactoryOperator::createComponent(FExComponentParams* params)
{
	if (params->isComponent())
	return static_cast<ExSimComponent*>(Component.factoryMethod(params));
	return nullptr;
}

void ExFactoryOperator::deleteComponent(ExSimComponent* component)
{
	ExSimComplex* complex = component->getBasis();
	complex->removeComponent(component);
	delete component;
}

void ExFactoryOperator::deleteComplex(ExSimComplex* params)
{
	delete params;
}


ExSimComplex* ExFactoryOperator::createComplex(FExComplexParams* params)
{
	return (Complex.factoryMethod(params));
}
