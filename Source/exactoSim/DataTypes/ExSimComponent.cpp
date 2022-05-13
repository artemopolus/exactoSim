#include "ExSimComponent.h"



bool ExSimComponent::getConstraintNames(TArray<FString>* names)
{
if (this->Constraints.Num() > 0)
	{
		for(const auto ctr : this->Constraints)
		{
			names->Add(ctr->getName());
		}
		return true;
	}
	return false;
}

bool ExSimComponent::addConstraint(FString name)
{
	return false;
}

bool ExSimComponent::removeConstraint(FString name)
{
	return false;
}

FString ExSimComponent::getComplexName()
{
	FString out = "";
	if(Basis)
		out = Basis->getName();
	return out;
}


ExSimComplex::~ExSimComplex()
{
	if (Basis)
		delete Basis;
	for (int i = 0; i < Components.Num(); i++)
		delete Components[i];
}

ExSimComponent* ExSimComplex::getComponent(FString name)
{
	for (auto component : Components)
	{
		if (component->getName() == name)
			return component;
	}
	return nullptr;
}
