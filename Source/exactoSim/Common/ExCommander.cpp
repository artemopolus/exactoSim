#include "ExCommander.h"


ExCreate::ExCreate(FExConstraintParams* trg)
{
	Params = trg;
	FExConstraintParams * tmp = new FExConstraintParams();
	*tmp = *trg;
	Store = tmp;
}

ExCreate::ExCreate(FExComponentParams* trg)
{
	//TODO: component creation
}

ExCreate::ExCreate(FExComplexParams* trg)
{
	//todo: complex creation
}

ExCreate::~ExCreate()
{
	if (Store) delete Store;
}

void ExCreate::execute()
{
	if (!Params)
	{
		if (Params->getType()==EnExParamTypes::CONSTRAINT)
		{
			FExConstraintParams * src = static_cast<FExConstraintParams*>(Store);
			FExConstraintParams * trg = static_cast<FExConstraintParams*>(Params);
			*trg = *src;
		}
		else if (Params->getType()==EnExParamTypes::COMPONENT)
		{
			
		}
		else if (Params->getType()==EnExParamTypes::COMPLEX)
		{
			
		}
	}
	Params->markToCreate();
}

void ExCreate::unExecute()
{
	Params->markToDelete();
}

ExDelete::ExDelete(FExConstraintParams* trg)
{
}

ExDelete::ExDelete(FExComponentParams* trg)
{
}

ExDelete::ExDelete(FExComplexParams* trg)
{
}

ExDelete::~ExDelete()
{
}

void ExDelete::execute()
{
}

void ExDelete::unExecute()
{
}

void ExDelete::getTarget(FExCommonParams* trg)
{
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, FVector vec)
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

void ExCommander::updateConstraint(EnExConstraintParamNames type, FString str)
{
	if (( EnExConstraintParamNames::vector_start < type)&&(type <EnExConstraintParamNames::string_start))
	{
		const FVector vec = ExConvert::getVecFromStr(str);
		updateConstraint(type, vec);
	}
	else if (( EnExConstraintParamNames::string_start < type)&&(type <EnExConstraintParamNames::spec_start))
	{
		if(!ActiveConstraint)
			return;
		Command = new ExUpdateConstraintString(ActiveConstraint, type, str);
		Command->execute();
		DoneCommands.Add(Command);
	}
	else if (EnExConstraintParamNames::float_start < type && type < EnExConstraintParamNames::int_start)
	{
		const float val = ExConvert::getFloatFromStr(str);
		updateConstraint(type, val);
	}
	else if (EnExConstraintParamNames::int_start < type && type < EnExConstraintParamNames::opt_end)
	{
		if (type == EnExConstraintParamNames::enables_spring)
		{
			uint8_t val;
			ExConvert::getIntFromBoolStr(str, & val);
			updateConstraint(type, val);
		}
	}
	else if (type == EnExConstraintParamNames::constraint_t)
	{
		const ExSimPhyzHelpers::Constraint val = ExSimPhyzHelpers::getConstraintFromName(str);	
		updateConstraint(type, val);
	}
}

void ExCommander::executeCommand()
{
	Command->execute();
	DoneCommands.Add(Command);
	if (DoneCommands.Num() >= DoneCommandMax)
	{
		Command = DoneCommands[0];
		DoneCommands.Remove(Command);
		delete Command;
	}
	FExCommonParams* params = nullptr;
	Command->getTarget(params);
	EcEvOnCommand.Broadcast(params);
}

void ExCommander::undo()
{
	if (DoneCommands.Num() == 0)
	{}
	else
	{
		Command = DoneCommands.Pop();
		// DoneCommands.pop_back();
		Command->unExecute();
		FExCommonParams * params = nullptr;
		Command->getTarget(params);
		EcEvOnCommand.Broadcast(params);
		delete Command;
	}
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, float val)
{
	if (!ActiveConstraint)
		return;
	Command = new ExUpdateConstraintFloat(ActiveConstraint, type, val);
	executeCommand();
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, int val)
{
	if (!ActiveConstraint)
		return;
	Command = new ExUpdateConstraintInt(ActiveConstraint, type, val);
	executeCommand();
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, ExSimPhyzHelpers::Constraint val)
{
	if (!ActiveConstraint)
		return;
	Command = new ExUpdateConstraintType(ActiveConstraint, type, val);
	executeCommand();
}

void ExCommander::updateComponent(FExComponentParams* component, EnExComponentParamNames type, FString val)
{
	if (EnExComponentParamNames::AA_VECTOR_START < type && type < EnExComponentParamNames::BA_ROTATOR_START )
	{
		updateComponent(component, type, ExConvert::getVecFromStr(val));
	}
	else if (EnExComponentParamNames::BA_ROTATOR_START < type && type < EnExComponentParamNames::CA_STRING_START )
	{
		updateComponent(component, type, ExConvert::getRotFromStr(val));
	}
	else if (EnExComponentParamNames::CA_STRING_START < type && type < EnExComponentParamNames::DA_FLOAT_START )
	{
		Command =new ExUpdComponentString(component, type, val);
		executeCommand();
	}
	else if (EnExComponentParamNames::DA_FLOAT_START < type && type < EnExComponentParamNames::ZZ_OPT_END )
	{
		updateComponent(component, type, ExConvert::getFloatFromStr(val));
	}
}

void ExCommander::updateComponent(FExComponentParams* component, EnExComponentParamNames type, FVector val)
{
	Command = new ExUpdComponentVector(component, type, val);
	executeCommand();
}

void ExCommander::updateComponent(FExComponentParams* component, EnExComponentParamNames type, FRotator val)
{
	Command = new ExUpdComponentRotator(component, type, val);
	executeCommand();
}

void ExCommander::updateComponent(FExComponentParams* component, EnExComponentParamNames type, float val)
{
	Command = new ExUpdComponentFloat(component, type, val);
	executeCommand();
}

void ExCommander::updateComplex(FExComplexParams* complex, EnExComplexParamNames type, FString val)
{
	if (EnExComplexParamNames::CA_STRING_START < type && type < EnExComplexParamNames::DA_FLOAT_START)
	{
		Command = new ExUpdComplexString(complex, type, val);
		executeCommand();
	}
}

void ExCommander::createConstraint(FExConstraintParams* constraint)

{
	Command = new ExCreate(constraint);
	executeCommand();
}

void ExCommander::createComponent(FExComponentParams* component)
{
	Command = new ExCreate(component);
	executeCommand();
}


