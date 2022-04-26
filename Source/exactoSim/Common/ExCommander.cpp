#include "ExCommander.h"


void ExStringPack::setActive(FExConstraintParams* target, EConstraintParamNames type, FString vec)
{
	Target = target;
	Type = type;
	Cur = vec;
}

bool ExStringPack::revert()
{
	return ExConvert::updateParams(Target, Type, Old);
}

bool ExStringPack::update()
{
	ExConvert::getParams(Target, Type, &Old);
	return ExConvert::updateParams(Target, Type, Cur);
}


void ExVectorPack::setActive(FExConstraintParams* target, EConstraintParamNames type, FVector vec)
{
	Target = target;
	Type = type;
	Cur = vec;
}

bool ExVectorPack::update()
{
	ExConvert::getParams(Target, Type, &Old);
	return ExConvert::updateParams(Target, Type, Cur);
}

bool ExVectorPack::revert()
{
	return ExConvert::updateParams(Target, Type, Old);
}

void ExConstraintPack::setActive(FExConstraintParams* target, EConstraintParamNames type, BulletHelpers::Constr vec)
{
	Target = target;
	Type = type;
	Cur = vec;
}

bool ExConstraintPack::update()
{
	ExConvert::getParams(Target, Type, &Old);
	return ExConvert::updateParams(Target, Type, Cur);
}

bool ExConstraintPack::revert()
{
	return ExConvert::updateParams(Target, Type, Old);
}

void ExFloatPack::setActive(FExConstraintParams* target, EConstraintParamNames type, float vec)
{
	Target = target;
	Type = type;
	Cur = vec;
}

bool ExFloatPack::update()
{
	ExConvert::getParams(Target, Type, &Old);
	return ExConvert::updateParams(Target, Type, Cur);
}

bool ExFloatPack::revert()
{
	return ExConvert::updateParams(Target, Type, Old);
}

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

