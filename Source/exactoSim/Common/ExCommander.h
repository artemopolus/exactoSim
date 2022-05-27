#pragma once
// #include <vector>

#include "exactoSim/ExactoPhysics.h"
#include "exactoSim/DataTypes/ExFactoryOperator.h"
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
	virtual ~ExBasicCommand(){	}
	virtual void execute() = 0; 
	virtual void unExecute() = 0;
	virtual void getTarget(FExCommonParams ** trg) = 0;
};
class EXACTOSIM_API ExUpdateConstraintString : public ExBasicCommand
{
	TExPack<FExConstraintParams, EnExConstraintParamNames, FString> Pack;
public:
	ExUpdateConstraintString(FExConstraintParams* target, EnExConstraintParamNames type, FString str){Pack.setActive(target, type, str);}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
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
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
};
//exacto Sim Component
class EXACTOSIM_API ExUpdComponentString : public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, FString> Pack;
public:
	ExUpdComponentString(FExComponentParams * trg, EnExComponentParamNames type, FString val){Pack.setActive(trg, type, val);}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
};
class EXACTOSIM_API ExUpdComponentVector: public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, FVector> Pack;
public:
    ExUpdComponentVector(FExComponentParams * trg, EnExComponentParamNames type, FVector val){Pack.setActive(trg, type, val);}
    virtual void execute() override { Pack.update(); }
    virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget( FExCommonParams** trg) override
    {
	    *trg = Pack.getTarget();
    	__nop();
    }
};
class EXACTOSIM_API ExUpdComponentRotator: public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, FRotator> Pack;
public:
    ExUpdComponentRotator(FExComponentParams * trg, EnExComponentParamNames type, FRotator val){Pack.setActive(trg, type, val);}
    virtual void execute() override { Pack.update(); }
    virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
};
class EXACTOSIM_API ExUpdComponentFloat: public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, float> Pack;
public:
    ExUpdComponentFloat(FExComponentParams * trg, EnExComponentParamNames type, float val){Pack.setActive(trg, type, val);}
    virtual void execute() override { Pack.update(); }
    virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
};
class EXACTOSIM_API ExUpdComponentInt: public ExBasicCommand
{
	TExPack<FExComponentParams, EnExComponentParamNames, int32> Pack;
public:
    ExUpdComponentInt(FExComponentParams * trg, EnExComponentParamNames type, int32 val){Pack.setActive(trg, type, val);}
    virtual void execute() override { Pack.update(); }
    virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
};
class EXACTOSIM_API ExUpdConstraintParent : public ExBasicCommand
{
protected:
	FExConstraintParams * Trg = nullptr;
	ExSimObject * Value = nullptr;
	ExSimObject * Store = nullptr;
public:
	ExUpdConstraintParent(FExConstraintParams * params, ExSimComponent * comp) : Trg(params), Value(comp){}
	virtual void execute() override{ Store = Trg->getDataPointer(); Trg->setDataPointer(Value);}
	virtual void unExecute() override {Trg->setDataPointer(Store);}
	virtual void getTarget(FExCommonParams** trg) override {*trg = Trg;}
};
class EXACTOSIM_API ExUpdConstraintTarget : public ExUpdConstraintParent
{
public:	
	ExUpdConstraintTarget(FExConstraintParams * params, ExSimComponent * comp) : ExUpdConstraintParent(params, comp){}
	virtual void execute() override{ Store = Trg->getAdditionalPointer(); Trg->setAdditionalPointer(Value);}
	virtual void unExecute() override {Trg->setAdditionalPointer(Store);}
};
//exacto Sim Complex
class EXACTOSIM_API ExUpdComplexString : public ExBasicCommand
{
	TExPack<FExComplexParams, EnExComplexParamNames, FString> Pack;
public:
	ExUpdComplexString(FExComplexParams * trg, EnExComplexParamNames type, FString val){Pack.setActive(trg, type, val);}
	virtual void execute() override { Pack.update(); }
	virtual void unExecute() override { Pack.revert(); }
	virtual void getTarget(FExCommonParams** trg) override{*trg = Pack.getTarget();}
};
// create Constraint, Component
class EXACTOSIM_API ExConstructor : public ExBasicCommand
{
	FExCommonParams * Params = nullptr; //for manipulating -- have to deleted outside
public:
	virtual ~ExConstructor() override {}
	explicit ExConstructor(FExCommonParams * trg) {Params = trg;}
	void construct(){Params->markToCreate();}
	void deconstruct(){Params->markToDelete();}
	void hide(){Params->markToHide();}
	void show(){Params->markToShow();}
	virtual void getTarget(FExCommonParams** trg) override {*trg = Params;}
};

class EXACTOSIM_API ExCreate : public ExConstructor
{
public:
	explicit ExCreate( FExConstraintParams *trg)	: ExConstructor(trg){}
	explicit ExCreate(FExComponentParams *trg)		: ExConstructor(trg){}
	explicit ExCreate(FExComplexParams *trg)		: ExConstructor(trg){}
	virtual void execute() override		{construct();	}
	virtual void unExecute() override	{deconstruct();	}
};
class EXACTOSIM_API ExDelete : public ExConstructor
{
public:
	explicit ExDelete(FExConstraintParams *trg)	: ExConstructor(trg){}
	explicit ExDelete(FExComponentParams *trg)	: ExConstructor(trg){}
	explicit ExDelete(FExComplexParams *trg)	: ExConstructor(trg){}
	virtual void execute() override		{deconstruct();	}
	virtual void unExecute() override	{construct();	}
};
class EXACTOSIM_API ExHide : public ExConstructor
{
public:
	explicit ExHide(FExCommonParams * trg): ExConstructor(trg){}
	virtual void execute() override{hide();}
	virtual void unExecute() override {show();}
};
class EXACTOSIM_API ExShow : public ExConstructor
{
public:
	explicit ExShow(FExCommonParams * trg): ExConstructor(trg){}
	virtual void execute() override{show();}
	virtual void unExecute() override {hide();}
};
DECLARE_MULTICAST_DELEGATE_OneParam(FEvExCommander, FExCommonParams * );

class EXACTOSIM_API ExCommander
{
	TArray<ExBasicCommand*> DoneCommands;
	ExBasicCommand * Command = nullptr;
	FExCommonParams * ActiveParam = nullptr;
	int DoneCommandMax = 5;
public:
	~ExCommander()
	{
		for (int32 i = 0; i < DoneCommands.Num(); i++)
		{
			auto * c = DoneCommands[i];
			delete c;
		}
		DoneCommands.Empty(); //don't know for what?
	}
	void setActiveObject(FExComplexParams * params){ActiveParam = params;}
	void setActiveObject(FExComponentParams * params){ActiveParam = params;}
	void setActiveObject(FExConstraintParams * constraint){	ActiveParam = constraint;}
	void updateConstraint( EnExConstraintParamNames type, FVector vec);
	void updateConstraint( FExConstraintParams * params, EnExConstraintParamNames type, FString str);
	void updateConstraint( EnExConstraintParamNames type, FString str);
	void updateConstraint( EnExConstraintParamNames type, float val);
	void updateConstraint( EnExConstraintParamNames type, int32 val);
	void updateConstraint( EnExConstraintParamNames type, ExSimPhyzHelpers::Constraint val);

	void updateComponent( EnExComponentParamNames type, FString val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, FString val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, FVector val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, FRotator val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, float val);
	void updateComponent(FExComponentParams * component, EnExComponentParamNames type, int32 val);

	void updateComplex(FExComplexParams * complex, EnExComplexParamNames, FString val);

	void setConstraintParent( ExSimComponent * trg);
	void setConstraintTarget( ExSimComponent * trg);


	void update(int type, FString val);
	void create();
	void remove();
	void hide();
	void show();

	void undo();

	void undo(FExCommonParams * params)
	{
		if (DoneCommands.Num())
		{
			Command = DoneCommands.Pop();
			Command->unExecute();
			Command->getTarget(&params);
			delete Command;			
		}
	}
private:
	void executeCommand();
	bool isConstraint();
	FExConstraintParams * getConstraint();
	bool isComponent();
	FExComponentParams * getComponent();
	bool isComplex();
	FExComplexParams * getComplex();

public:
	FEvExCommander EcEvOnCommand;
	
};


