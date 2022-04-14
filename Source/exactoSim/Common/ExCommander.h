#pragma once
#include <vector>

#include "exactoSim/ExactoPhysics.h"


class ExPack
{
	AExactoPhysics::es_constraint * Target = nullptr;
	AExactoPhysics::es_options_list Type = AExactoPhysics::vector_start;
	FVector Old;
	FVector Cur;
public:
	void setActive(AExactoPhysics::es_constraint * target);
	bool update(const AExactoPhysics::es_options_list new_type, const FVector new_vec);
	bool revert();
};

class ExBasicCommand
{
public:
	virtual ~ExBasicCommand(){}
	virtual void execute() = 0;
	virtual void unExecute() = 0;
};

class ExUpdateConstraintVector : public ExBasicCommand
{
	ExPack Pack;
	AExactoPhysics::es_options_list Type;
	FVector Vec = FVector::ZeroVector;
public:
	ExUpdateConstraintVector(AExactoPhysics::es_constraint * target, AExactoPhysics::es_options_list type, FVector vec) : Type(type), Vec(vec)
	{
		Pack.setActive(target);
	}

	virtual void execute() override
	{
		Pack.update(Type, Vec);
	}

	virtual void unExecute() override
	{
		Pack.revert();
	}
};

class ExCommander
{
	std::vector<ExBasicCommand*> DoneCommands;
	ExBasicCommand * Command = nullptr;
	AExactoPhysics::es_constraint * ActiveConstraint = nullptr;
public:
	void setActiveConstraint(AExactoPhysics::es_constraint * constraint)
	{
		ActiveConstraint = constraint;	
	}
	void updateConstraint( AExactoPhysics::es_options_list type, FVector vec)
	{
		if (!ActiveConstraint)
			return;
		Command = new ExUpdateConstraintVector(ActiveConstraint, type, vec);
		Command->execute();
		DoneCommands.push_back(Command);
	}
	void undo()
	{
		if (DoneCommands.size() == 0)
		{}
		else
		{
			Command = DoneCommands.back();
			DoneCommands.pop_back();
			Command->unExecute();
			delete Command;
		}
	}
	
	
};
