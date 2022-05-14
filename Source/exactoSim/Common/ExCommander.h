#pragma once
// #include <vector>

#include "exactoSim/ExactoPhysics.h"
#include "exactoSim/Utils/ExConvert.h"


template < typename Tparams, typename Ttype, typename Tval>
class EXACTOSIM_API TExPack
{
	Tparams* Target = nullptr;
	Ttype Type;
	Tval Cur;
	Tval Old;
public:
	void setActive(Tparams* target, Ttype type, Tval value);
	bool update();
	bool revert();
};

template <typename Tparams, typename Ttype, typename Tval>
void TExPack<Tparams, Ttype, Tval>::setActive(Tparams* target, Ttype type, Tval value)
{
	Target = target; Type = type; Cur = value;
}

template <typename Tparams, typename Ttype, typename Tval>
bool TExPack<Tparams, Ttype, Tval>::update()
{
	ExConvert::getParams(Target, Type, &Old);
	return ExConvert::updateParams(Target, Type, Cur);
}

template <typename Tparams, typename Ttype, typename Tval>
bool TExPack<Tparams, Ttype, Tval>::revert()
{
	return ExConvert::updateParams(Target, Type, Old);
}


// template <typename Tval>
// void TExPack<Tval>::setActive(FExConstraintParams* target, EConstraintParamNames type, Tval vec)
// {
// 	Target = target;
// 	Type = type;
// 	Cur = vec;
// }
//
// template <typename T>
// bool TExPack<T>::update()
// {
// 	ExConvert::getParams(Target, Type, &Old);
// 	return ExConvert::updateParams(Target, Type, Cur);
// }
//
// template <typename T>
// bool TExPack<T>::revert()
// {
// 	return ExConvert::updateParams(Target, Type, Old);
// }


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
	TExPack<FExConstraintParams, EnExConstraintParamNames, FString> Pack;
public:
	ExUpdateConstraintString(FExConstraintParams* target, EnExConstraintParamNames type, FString str)
	{
		Pack.setActive(target, type, str);
	}

	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
};

class EXACTOSIM_API ExUpdateConstraintVector : public ExBasicCommand
{
	TExPack<FExConstraintParams, EnExConstraintParamNames, FVector> Pack;
public:
	ExUpdateConstraintVector(FExConstraintParams* target, EnExConstraintParamNames type, FVector vec)
	{
		Pack.setActive(target, type, vec);
	}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
};
class EXACTOSIM_API ExUpdateConstraintFloat : public ExBasicCommand
{
	TExPack<FExConstraintParams, EnExConstraintParamNames, float> Pack;
public:
	ExUpdateConstraintFloat(FExConstraintParams* target, EnExConstraintParamNames type, float vec)
	{
		Pack.setActive(target, type, vec);
	}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
};
class EXACTOSIM_API ExUpdateConstraintInt : public ExBasicCommand
{
	TExPack<FExConstraintParams, EnExConstraintParamNames, int> Pack;
public:
	ExUpdateConstraintInt(FExConstraintParams* target, EnExConstraintParamNames type, int vec)
	{
		Pack.setActive(target, type, vec);
	}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
};
class EXACTOSIM_API ExUpdateConstraintType : public ExBasicCommand
{
	TExPack<FExConstraintParams, EnExConstraintParamNames, ExSimPhyzHelpers::Constraint> Pack;
public:
	ExUpdateConstraintType(FExConstraintParams* target, EnExConstraintParamNames type, ExSimPhyzHelpers::Constraint vec)
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
	void updateConstraint( EnExConstraintParamNames type, FVector vec);
	void updateConstraint( EnExConstraintParamNames type, FString str);
	void updateConstraint( EnExConstraintParamNames type, float val);
	void updateConstraint( EnExConstraintParamNames type, int val);
	void updateConstraint( EnExConstraintParamNames type, ExSimPhyzHelpers::Constraint val);

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
private:
	void executeCommand();
	
	
};


