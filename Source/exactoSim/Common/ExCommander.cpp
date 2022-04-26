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
}

