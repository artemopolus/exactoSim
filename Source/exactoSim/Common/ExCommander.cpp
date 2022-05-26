#include "ExCommander.h"






void ExCommander::updateConstraint(EnExConstraintParamNames type, FVector vec)
{
	if (!isConstraint())
		return;
	Command = new ExUpdateConstraintVector(getConstraint(), type, vec);
	Command->execute();
	DoneCommands.Add(Command);
	if (DoneCommands.Num() >= DoneCommandMax)
	{
		Command = DoneCommands[0];
		DoneCommands.Remove(Command);
		delete Command;
	}
}

void ExCommander::updateConstraint(FExConstraintParams* params, EnExConstraintParamNames type, FString str)
{
	ActiveParam = params;
	updateConstraint(type, str);
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, FString str)
{
	if (( EnExConstraintParamNames::VECTOR_START < type)&&(type <EnExConstraintParamNames::STRING_START))
	{
		const FVector vec = ExConvert::getVecFromStr(str);
		updateConstraint(type, vec);
	}
	else if (( EnExConstraintParamNames::STRING_START < type)&&(type <EnExConstraintParamNames::SPEC_START))
	{
		if(!isConstraint())
			return;
		Command = new ExUpdateConstraintString(getConstraint(), type, str);
		Command->execute();
		DoneCommands.Add(Command);
	}
	else if (EnExConstraintParamNames::FLOAT_START < type && type < EnExConstraintParamNames::INT_START)
	{
		const float val = ExConvert::getFloatFromStr(str);
		updateConstraint(type, val);
	}
	else if (EnExConstraintParamNames::INT_START < type && type < EnExConstraintParamNames::opt_end)
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

	if (DoneCommands.Num() >= DoneCommandMax)
	{
		auto first = DoneCommands[0];
		FExCommonParams * pparam = nullptr;
		first->getTarget(&pparam);
		if(pparam->isComponent() && pparam->isMarkedToHide())
		{
			pparam->markToDelete();
			EcEvOnCommand.Broadcast(pparam);
		}
		DoneCommands.Remove(first);
		delete first;
	}
	Command->execute();
	DoneCommands.Add(Command);
	FExCommonParams* params = nullptr;
	Command->getTarget(&params);
	EcEvOnCommand.Broadcast(params);
}

bool ExCommander::isConstraint()
{
	if (!ActiveParam)
		return false;
	if (ActiveParam->getType() == EnExParamTypes::CONSTRAINT)
		return true;
	return false;
}

FExConstraintParams* ExCommander::getConstraint()
{
	return static_cast<FExConstraintParams*>(ActiveParam);
}

bool ExCommander::isComponent()
{
	if (!ActiveParam)
		return false;
	if (ActiveParam->getType() == EnExParamTypes::COMPONENT)
		return true;
	return false;
}

FExComponentParams* ExCommander::getComponent()
{
	return static_cast<FExComponentParams*>(ActiveParam);
}

bool ExCommander::isComplex()
{
	if (!ActiveParam)
		return false;
	if (ActiveParam->getType() == EnExParamTypes::COMPLEX)
		return true;
	return false;
}

FExComplexParams* ExCommander::getComplex()
{
	return static_cast<FExComplexParams*>(ActiveParam);
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
		Command->getTarget(&params);
		EcEvOnCommand.Broadcast(params);
		delete Command;
	}
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, float val)
{
	if (!isConstraint())
		return;
	Command = new ExUpdateConstraintFloat(getConstraint(), type, val);
	executeCommand();
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, int32 val)
{
	if (!isConstraint())
		return;
	Command = new ExUpdateConstraintInt(getConstraint(), type, val);
	executeCommand();
}

void ExCommander::updateConstraint(EnExConstraintParamNames type, ExSimPhyzHelpers::Constraint val)
{
	if (!isConstraint())
		return;
	Command = new ExUpdateConstraintType(getConstraint(), type, val);
	executeCommand();
}

void ExCommander::updateComponent(EnExComponentParamNames type, FString val)
{
	if (isComponent())
		updateComponent(getComponent(),type, val);
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
	else if (EnExComponentParamNames::DA_FLOAT_START < type && type < EnExComponentParamNames::EA_INT_START )
	{
		updateComponent(component, type, ExConvert::getFloatFromStr(val));
	}
	else if (EnExComponentParamNames::EA_INT_START < type && type < EnExComponentParamNames::ZZ_OPT_END )
	{
		updateComponent(component,type, ExConvert::getIntFromStr(val));
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

void ExCommander::updateComponent(FExComponentParams* component, EnExComponentParamNames type, int32 val)
{
	Command = new ExUpdComponentInt(component, type, val);
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

void ExCommander::update(int type, FString val)
{
	if (isConstraint())
		updateConstraint(static_cast<EnExConstraintParamNames>(type), val);
	else if(isComponent())
		updateComponent(getComponent(),static_cast<EnExComponentParamNames>(type), val);
	else if(isComplex())
		updateComplex(getComplex(),static_cast<EnExComplexParamNames>(type), val);
}

void ExCommander::create()
{
	if (isConstraint())
		Command = new ExCreate(getConstraint());
	else if(isComponent())
		Command = new ExCreate(getComponent());
	else if(isComplex())
		Command = new ExCreate(getComplex());
	executeCommand();
}

void ExCommander::remove()
{
	if(isConstraint())
		Command = new ExDelete(getConstraint());
	else if (isComponent())
		Command = new ExDelete(getComponent());
	else if(isComplex())
		Command = new ExDelete(getComplex());
	else return;
	executeCommand();
}

void ExCommander::hide()
{
	if(isComponent()) // only component
	{
		Command = new ExHide(getComponent());
		executeCommand();
	}
}

void ExCommander::show()
{
	if(isComponent()) // only component
	{
		Command = new ExShow(getComponent());
		executeCommand();
	}
}

