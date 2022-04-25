#pragma once
// #include <vector>

#include "exactoSim/ExactoPhysics.h"
#include "exactoSim/Utils/ExConvert.h"

//TODO: use template for pack
class EXACTOSIM_API ExStringPack
{
		FExConstraintParams * Target = nullptr;
    	EConstraintParamNames Type = EConstraintParamNames::vector_start;
    	FString Old;
    	FString Cur;
    public:
    	void setActive(FExConstraintParams * target, EConstraintParamNames type, FString vec)
    	{
    		Target = target;
    		Type = type;
    		Cur = vec;
    	}
    	bool update(const EConstraintParamNames new_type, const FString new_vec);
    	bool revert();
    	bool update()
    	{
    		return update(Type, Cur);
    	}
};
class EXACTOSIM_API ExVectorPack
{
	FExConstraintParams * Target = nullptr;
	EConstraintParamNames Type = EConstraintParamNames::vector_start;
	FVector Old;
	FVector Cur;
public:
	void setActive(FExConstraintParams * target, EConstraintParamNames type, FVector vec)
	{
		Target = target;
		Type = type;
		Cur = vec;
	}
	bool update(const EConstraintParamNames new_type, const FVector new_vec);
	bool update()
	{
		return update(Type, Cur);
	}
	bool revert();
};

class EXACTOSIM_API ExBasicCommand
{
public:
	virtual ~ExBasicCommand(){}
	virtual void execute() = 0;
	virtual void unExecute() = 0;
};
class EXACTOSIM_API ExUpdateConstraintString : public ExBasicCommand
{
	ExStringPack Pack;
public:
	ExUpdateConstraintString(FExConstraintParams * target, EConstraintParamNames type, FString str)
	{
		Pack.setActive(target, type, str);
	}
	virtual void execute() override
	{
		Pack.update();
	}
	virtual void unExecute() override
	{
		Pack.revert();
	}
};
class EXACTOSIM_API ExUpdateConstraintVector : public ExBasicCommand
{
	ExVectorPack Pack;
public:
	ExUpdateConstraintVector(FExConstraintParams * target, EConstraintParamNames type, FVector vec) 
	{
		Pack.setActive(target, type, vec);
	}

	virtual void execute() override
	{
		Pack.update();
	}

	virtual void unExecute() override
	{
		Pack.revert();
	}
};

class EXACTOSIM_API ExCommander
{
	TArray<ExBasicCommand*> DoneCommands;
	ExBasicCommand * Command = nullptr;
	FExConstraintParams * ActiveConstraint = nullptr;
	int DoneCommandMax = 20;
public:
	ExCommander(){}
	~ExCommander()
	{
		for (int i = 0; i < DoneCommands.Num(); i++)
		{
			auto * c = DoneCommands[i];
			delete c;
		}
		DoneCommands.Empty(); //don't know for what?
	}
	void setActiveConstraint(FExConstraintParams * constraint)
	{
		ActiveConstraint = constraint;	
	}
	void updateConstraint( EConstraintParamNames type, FVector vec)
	{
		if (!ActiveConstraint)
			return;
		Command = new ExUpdateConstraintVector(ActiveConstraint, type, vec);
		Command->execute();
		DoneCommands.Add(Command);
		if (DoneCommands.Num() >= DoneCommandMax)
		{
			Command = DoneCommands[0];
			DoneCommands.Remove(Command);
			delete Command;
		}
	}
	void updateConstraint(EConstraintParamNames type, FString str)
	{
		if (( EConstraintParamNames::vector_start < type)&&(type <EConstraintParamNames::string_start))
		{
			const FVector vec = ExConvert::getVecFromStr(str);
			updateConstraint(type, vec);
		}
		else if (( EConstraintParamNames::string_start < type)&&(type <EConstraintParamNames::spec_start))
		{
			if(!ActiveConstraint)
				return;
			Command = new ExUpdateConstraintString(ActiveConstraint, type, str);
			Command->execute();
			DoneCommands.Add(Command);
		}
	}
	void undo()
	{
		if (DoneCommands.Num() == 0)
		{}
		else
		{
			Command = DoneCommands.Pop();
			// DoneCommands.pop_back();
			Command->unExecute();
			delete Command;
		}
	}
	
	
};
