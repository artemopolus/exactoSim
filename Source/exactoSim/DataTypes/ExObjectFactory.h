#pragma once
#include "ExSimComponent.h"

class EXACTOSIM_API ExObjectFactory
{
public:
	virtual ~ExObjectFactory() = default;
	virtual ExSimObject * factoryMethod( FExCommonParams * params) = 0;
};

class EXACTOSIM_API ExConstraintPairFactory : public ExObjectFactory
{
public:
	virtual ExSimObject* factoryMethod(FExCommonParams * params) override
	{
		ExSimConstraintPair * pair = new ExSimConstraintPair();
		pair->setParams(static_cast<FExConstraintParams*>(params));
		return pair;
	}
	ExSimConstraintPair * factoryMethod(ExSimComponent * par, FExConstraintParams * params)
	{
		ExSimConstraintPair * pair =  new ExSimConstraintPair(par, params);
		par->addConstraint(pair);
		return pair;
	}
};

class EXACTOSIM_API ExComponentFactory : public ExObjectFactory
{
public:
	virtual ExSimObject* factoryMethod(FExCommonParams * params) override
	{
		return new ExSimComponent(params);
	}
};

class EXACTOSIM_API ExComplexFactory : public ExObjectFactory
{
public:
	virtual ExSimObject* factoryMethod(FExCommonParams * params) override
	{
		return  new ExSimComplex();
	}
	ExSimComplex * factoryMethod(FExComplexParams * params)
	{
		ExSimComplex * complex = new ExSimComplex();
		complex->setParams(params);
		return  complex;
	}
};

