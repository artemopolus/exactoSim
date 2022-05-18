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
	FExCommonParams * getTarget() const;
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

template <typename Tparams, typename Ttype, typename Tval>
FExCommonParams* TExPack<Tparams, Ttype, Tval>::getTarget() const
{
	return static_cast<FExCommonParams*>(Target);
}


//COMMANDS

class EXACTOSIM_API ExBasicCommand
{
public:
	virtual ~ExBasicCommand()
	{
	}
	virtual void execute() = 0; 
	virtual void unExecute() = 0;
	virtual void getTarget(FExCommonParams * trg) = 0; 
};
class EXACTOSIM_API ExUpdateConstraintString : public ExBasicCommand
{
	TExPack<FExConstraintParams, EnExConstraintParamNames, FString> Pack;
public:
	ExUpdateConstraintString(FExConstraintParams* target, EnExConstraintParamNames type, FString str){Pack.setActive(target, type, str);}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
};
//exacto Sim Component
class EXACTOSIM_API ExUpdComponentString : public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, FString> Pack;
public:
	ExUpdComponentString(FExComponentParams * trg, EnExComponentParamNames type, FString val){Pack.setActive(trg, type, val);}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
};
class EXACTOSIM_API ExUpdComponentVector: public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, FVector> Pack;
public:
    ExUpdComponentVector(FExComponentParams * trg, EnExComponentParamNames type, FVector val){Pack.setActive(trg, type, val);}
    virtual void execute() override { Pack.update(); }
    virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
};
class EXACTOSIM_API ExUpdComponentRotator: public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, FRotator> Pack;
public:
    ExUpdComponentRotator(FExComponentParams * trg, EnExComponentParamNames type, FRotator val){Pack.setActive(trg, type, val);}
    virtual void execute() override { Pack.update(); }
    virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
};
class EXACTOSIM_API ExUpdComponentFloat: public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, float> Pack;
public:
    ExUpdComponentFloat(FExComponentParams * trg, EnExComponentParamNames type, float val){Pack.setActive(trg, type, val);}
    virtual void execute() override { Pack.update(); }
    virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
};
//exacto Sim Complex
class EXACTOSIM_API ExUpdComplexString : public ExBasicCommand
{
	TExPack<FExComplexParams, EnExComplexParamNames, FString> Pack;
public:
	ExUpdComplexString(FExComplexParams * trg, EnExComplexParamNames type, FString val){Pack.setActive(trg, type, val);}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams* trg) override{trg = Pack.getTarget();}
};
// create Constraint, Component
class EXACTOSIM_API ExCreate : public ExBasicCommand
{
	FExCommonParams * Params = nullptr; //for manipulating -- have to deleted outside
	FExCommonParams * Store = nullptr; // for storing data
public:
	explicit ExCreate(FExConstraintParams* trg);
	explicit ExCreate(FExComponentParams * trg);
	explicit ExCreate(FExComplexParams * trg);
	virtual ~ExCreate() override;
	virtual void execute() override;
	virtual void unExecute() override;
	virtual void getTarget(FExCommonParams* trg) override{trg = Params;}
	
};
class EXACTOSIM_API ExDelete : public ExBasicCommand
{
	FExCommonParams * Params = nullptr; //for manipulating -- have to deleted outside
	FExCommonParams * Store = nullptr; // for storing data
public:
	explicit ExDelete(FExConstraintParams * trg);
	explicit ExDelete(FExComponentParams * trg);
	explicit ExDelete(FExComplexParams * trg);
	virtual ~ExDelete() override;
	virtual void execute() override;
	virtual void unExecute() override;
	virtual void getTarget(FExCommonParams* trg) override;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FEvExCommander, FExCommonParams * );

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
		for (int32 i = 0; i < DoneCommands.Num(); i++)
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

	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, FString val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, FVector val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, FRotator val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, float val);

	void updateComplex(FExComplexParams * complex, EnExComplexParamNames, FString val);


	void createConstraint(FExConstraintParams * constraint);
	void createComponent(FExComponentParams * component);
	

	void undo();

	void undo(FExCommonParams * params)
	{
		if (DoneCommands.Num())
		{
			Command = DoneCommands.Pop();
			Command->unExecute();
			Command->getTarget(params);
			delete Command;			
		}
	}
private:
	void executeCommand();
public:
	FEvExCommander EcEvOnCommand;
	
};


