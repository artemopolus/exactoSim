#include "ExSimComponent.h"


void ExSimConstraintPair::update()
{
	this->Name = Params->name_constraint;
	this->Type = Params->constr_type;
}

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


void ExSimComponent::setBasis(TArray<ExSimComplex*>* list)
{
	if (Basis && Basis->getName() == Params->BasisName)
		return;
	for (auto & complex : *list)
	{
		if (complex->getName() == Params->BasisName)
		{
			complex->addComponent(this);
			return;;
		}
	}
}

void ExSimComponent::update()
{
	this->Name = Params->Name;
	this->Path = Params->Path;
}

ExSimComplex::~ExSimComplex()
{
	if (Basis)
		delete Basis;
	for (int i = 0; i < Components.Num(); i++)
		delete Components[i];
}

void ExSimComplex::addComponent(ExSimComponent* component)
{
	ExSimComplex * prev = component->getBasis();
	prev->removeComponent(component);
	component->setBasis(this);
	Components.Add(component);
}

void ExSimComplex::removeComponent(ExSimComponent* component)
{
	Components.Remove(component);
}

void ExSimComplex::addComponents(TArray<ExSimComponent*> * targets)
{
	for(auto & comp : *targets)
	{
		if (Params->ComponentNames.Find(comp->getName()))
			this->addComponent(comp);
	}
}

void ExSimComplex::moveComponents(ExSimComplex* trg)
{
	for(int32 i = 0; i < Components.Num(); i++)
	{
		trg->addComponent(Components[i]);
	}
	static_assert(Components.Num() == 0);
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
void ExSimComplex::update()
{
	const auto component = getComponent(Params->BasisName);
	if (component)
	{
		setBasis(component);
	}
	this->Name = Params->Name;
}
