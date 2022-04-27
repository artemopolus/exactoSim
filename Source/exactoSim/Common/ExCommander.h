#pragma once
// #include <vector>

#include "exactoSim/ExactoPhysics.h"
#include "exactoSim/Utils/ExConvert.h"


template<typename T>
class EXACTOSIM_API TExPack 
{
	FExConstraintParams * Target = nullptr;
	EConstraintParamNames Type = EConstraintParamNames::vector_start;
	T Cur;
	T Old;
public:
	void setActive(FExConstraintParams * target, EConstraintParamNames type, T vec) ;
	bool update() ;
	bool revert() ;
};

template <typename T>
void TExPack<T>::setActive(FExConstraintParams* target, EConstraintParamNames type, T vec)
{
	Target = target;
	Type = type;
	Cur = vec;
}

template <typename T>
bool TExPack<T>::update()
{
	ExConvert::getParams(Target, Type, &Old);
	return ExConvert::updateParams(Target, Type, Cur);
}

template <typename T>
bool TExPack<T>::revert()
{
	return ExConvert::updateParams(Target, Type, Old);
}



class EXACTOSIM_API ExBasicCommand
{
public:
	virtual ~ExBasicCommand()
	{
	}
	virtual void execute() = 0; 
	virtual void unExecute() = 0; 
};
class EXACTOSIM_API ExUpdateConstraintString : public ExBasicCommand
{
	TExPack<FString> Pack;
public:
	ExUpdateConstraintString(FExConstraintParams* target, EConstraintParamNames type, FString str)
	{
		Pack.setActive(target, type, str);
	}

	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
};

class EXACTOSIM_API ExUpdateConstraintVector : public ExBasicCommand
{
	TExPack<FVector> Pack;
public:
	ExUpdateConstraintVector(FExConstraintParams* target, EConstraintParamNames type, FVector vec)
	{
		Pack.setActive(target, type, vec);
	}

	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
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
	void updateConstraint( EConstraintParamNames type, FVector vec);

	void updateConstraint(EConstraintParamNames type, FString str);

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
