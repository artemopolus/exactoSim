#include "ExSimComponent.h"


void ExSimConstraintPair::update()
{
	this->Name = Params->name_constraint;
	this->Type = Params->constr_type;
	ExSimComponent * parent = static_cast<ExSimComponent*>(Params->getDataPointer());
	
}

bool ExSimComponent::getConstraintNames(TArray<FString>* names)
{
	if (this->Constraints.Num() > 0)
	{
		for (const auto ctr : this->Constraints)
		{
			names->Add(ctr->getName());
		}
		return true;
	}
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

void ExSimComponent::addConstraint(ExSimConstraintPair* constr)
{
	Constraints.Add(constr);
	Params->ConstraintNames.Add(constr->getName());
}

void ExSimComponent::removeConstraint(ExSimConstraintPair* constr)
{
	Constraints.Remove(constr);
	Params->ConstraintNames.Remove(constr->getName());
}

void ExSimComponent::update()
{
	this->Name = Params->Name;
	this->Path = Params->Path;
	ExSimConstraintPair * pair = static_cast<ExSimConstraintPair*>(Params->getDataPointer());
	if (pair && pair->getObjType() == EnExParamTypes::CONSTRAINT)
	{
		addConstraint(pair);
	}
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
	if (!Components.Num())
		setBasis(component);
	Components.Add(component);
	Params->ComponentNames.Add(component->getName());
}

void ExSimComplex::removeComponent(ExSimComponent* component)
{
	Components.Remove(component);
	Params->ComponentNames.Remove(component->getName());
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
	check(Components.Num() == 0);
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
	if (BasisName != Params->BasisName)
	{
		const auto component = getComponent(Params->BasisName);
		if (component)
		{
			BasisName = component->getName();
			setBasis(component);
		}
	}
	ExSimComponent * comp = static_cast<ExSimComponent*>(Params->getDataPointer());
	if (comp && comp->getObjType() == EnExParamTypes::COMPONENT)
	{
		addComponent(comp);
	}
	
	if (Name != Params->Name)
		this->Name = Params->Name;
}

	
