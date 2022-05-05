#include "ExCommander.h"




void ExCommander::updateConstraint(EConstraintParamNames type, FVector vec)
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

void ExCommander::updateConstraint(EConstraintParamNames type, FString str)
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
	else if (EConstraintParamNames::float_start < type && type < EConstraintParamNames::int_start)
	{
		const float val = ExConvert::getFloatFromStr(str);
		updateConstraint(type, val);
	}
	else if (EConstraintParamNames::int_start < type && type < EConstraintParamNames::opt_end)
	{
		if (type == EConstraintParamNames::enables_spring)
		{
			uint8_t val;
			ExConvert::getIntFromBoolStr(str, & val);
			updateConstraint(type, val);
		}
	}
	else if (type == EConstraintParamNames::constraint_t)
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
}

void ExCommander::updateConstraint(EConstraintParamNames type, float val)
{
	if (!ActiveConstraint)
		return;
	Command = new ExUpdateConstraintFloat(ActiveConstraint, type, val);
	executeCommand();
}

void ExCommander::updateConstraint(EConstraintParamNames type, int val)
{
	if (!ActiveConstraint)
		return;
	Command = new ExUpdateConstraintInt(ActiveConstraint, type, val);
	executeCommand();
}

void ExCommander::updateConstraint(EConstraintParamNames type, ExSimPhyzHelpers::Constraint val)
{
	if (!ActiveConstraint)
		return;
	Command = new ExUpdateConstraintType(ActiveConstraint, type, val);
	executeCommand();
}

