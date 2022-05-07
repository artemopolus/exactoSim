#include "ExConvert.h"

FString ExConvert::getStrFromVec(FVector vec)
{
	return FString::SanitizeFloat(vec.X) + TEXT(" ;") + FString::SanitizeFloat(vec.Y) + TEXT(" ;") + FString::SanitizeFloat(vec.Z);
}

FString ExConvert::getStrFromRot(FRotator rot)
{
	return FString::SanitizeFloat(rot.Pitch) + TEXT(" ;") + FString::SanitizeFloat(rot.Roll) + TEXT(" ;") + FString::SanitizeFloat(rot.Yaw);
}

FString ExConvert::getStrFromFloat(float val)
{
	return FString::SanitizeFloat(val);
}

FString ExConvert::getBoolStrFromInt(uint8_t val, int last)
{
	FString out = "";
	for(int i = 1, k = 0;k < last; i*=2, k++)
	{
		if(val&i)
			out += " 1;";
		else
			out += " 0;";
	}
	return out;
}
bool ExConvert::getBoolArrayFromInt(uint8_t val, TArray<bool>* out, int last)
{
	if (val == 0)
		return false;
	for (int i = 1, k = 0; k < last; i *= 2, k++)
	{
		if (val & i)
			out->Add(true);
		else
			out->Add(false);
	}
	return true;
}
bool ExConvert::getIntFromBoolStr(FString str, uint8_t* out)
{
	TArray<FString> values;
	FString blank = str;
	blank.RemoveSpacesInline();
	blank.ParseIntoArray(values, TEXT(";"), true);
	int i = 1;
	*out = 0;
	if (!values.Num())
		return false;
	for (auto v : values)
	{
		if (v == "1")
		{
			*out += i;
			i *= 2;
		}
		else if (v == "0")
			i *= 2;
		else return false;
	}
	return true;
}



FVector ExConvert::getVecFromStr(FString str)
{
	TArray<FString> values;
	FVector out(0,0,0);
	str.ParseIntoArray(values, TEXT(";"), true);
	if (values.Num() == 3)
	{
		out.X = FCString::Atof(*values[0]);	
		out.Y = FCString::Atof(*values[1]);	
		out.Z = FCString::Atof(*values[2]);
	}
	return out;
}

bool ExConvert::checkVecStr(const FString str)
{
	TArray<FString> values;
	FString blank = str;
	blank.RemoveSpacesInline();
	
	blank.ParseIntoArray(values, TEXT(";"), true);
	if (values.Num() == 3)
	{
		for (auto v : values)
		{
			TArray<FString> pp;
			v.ParseIntoArray(pp, TEXT("."), true);
			if ((pp.Num() == 2)||(pp.Num() == 1))
			{
				for (auto p : pp)
				{
					if(!p.IsNumeric())
						return false;
				}
			}
			else return false;
		}
		return true;
	}
	return false;
}

float ExConvert::getFloatFromStr(FString str)
{
	return FCString::Atof(*str);
}

bool ExConvert::updateParams(FExConstraintParams* trg, EConstraintParamNames type, FString val)
{
	if (type == EConstraintParamNames::constraint_name)
		trg->name_constraint = val;
	else if (type == EConstraintParamNames::parent_name)
		trg->name_p = val;
	else if (type == EConstraintParamNames::target_name)
		trg->name_t = val;
	else return false;
	return true;
}

bool ExConvert::updateParams(FExConstraintParams* trg, EConstraintParamNames type, FVector val)
{
	if (type == EConstraintParamNames::dump_ang)
		trg->dump_ang = val;
	else if (type == EConstraintParamNames::dump_lin)
		trg->dump_lin = val;
	else if (type == EConstraintParamNames::low_lim_ang)
		trg->low_lim_ang = val;
	else if (type == EConstraintParamNames::low_lim_lin)
		trg->low_lim_lin = val;
	else if (type == EConstraintParamNames::parent_axis)
		trg->axis_p = val;
	else if (type == EConstraintParamNames::parent_pivot)
		trg->pivot_p = val;
	else if (type == EConstraintParamNames::stiff_ang)
		trg->stiff_ang = val;
	else if (type == EConstraintParamNames::stiff_lin)
		trg->stiff_lin = val;
	else if (type == EConstraintParamNames::target_axis)
		trg->axis_t = val;
	else if (type == EConstraintParamNames::target_pivot)
		trg->pivot_t = val;
	else if (type == EConstraintParamNames::upp_lim_ang)
		trg->upp_lim_ang = val;
	else if (type == EConstraintParamNames::upp_lim_lin)
		trg->upp_lim_lin = val;
	else return false;
	return true;
}

bool ExConvert::updateParams(FExConstraintParams* trg, EConstraintParamNames type, ExSimPhyzHelpers::Constraint val)
{
	if (type == EConstraintParamNames::constraint_t)
		trg->constr_type = val;
	else
		return false;
	return true;
}

bool ExConvert::updateParams(FExConstraintParams* trg, EConstraintParamNames type, float val)
{
	if (type == EConstraintParamNames::impulse_clamp)
		trg->impulse_clamp = val;
	else if (type == EConstraintParamNames::tau)
		trg->tau = val;
	else return false;
	return true;
}

bool ExConvert::updateParams(FExConstraintParams* trg, EConstraintParamNames type, int val)
{
	if (type == EConstraintParamNames::enables_spring)
		trg->enables_spring = val;
	else return false;
	return true;
}

bool ExConvert::getParams(FExConstraintParams* src, EConstraintParamNames type, FString* trg)
{
	if (type == EConstraintParamNames::constraint_name)
		*trg = src->name_constraint;
	else if (type == EConstraintParamNames::parent_name)
		*trg = src->name_p;
	else if (type == EConstraintParamNames::target_name)
		*trg = src->name_t;
	else return false;
	return true;
}

bool ExConvert::getParams(FExConstraintParams* src, EConstraintParamNames type, FVector* trg)
{
	if (type == EConstraintParamNames::dump_ang)
		*trg = src->dump_ang;
	else if (type == EConstraintParamNames::dump_lin)
		*trg = src->dump_lin;
	else if (type == EConstraintParamNames::low_lim_ang)
		*trg = src->low_lim_ang;
	else if (type == EConstraintParamNames::low_lim_lin)
		*trg = src->low_lim_lin;
	else if (type == EConstraintParamNames::parent_axis)
		*trg = src->axis_p;
	else if (type == EConstraintParamNames::parent_pivot)
		*trg = src->pivot_p;
	else if (type == EConstraintParamNames::stiff_ang)
		*trg = src->stiff_ang;
	else if (type == EConstraintParamNames::stiff_lin)
		*trg = src->stiff_lin;
	else if (type == EConstraintParamNames::target_axis)
		*trg = src->axis_t;
	else if (type == EConstraintParamNames::target_pivot)
		*trg = src->pivot_t;
	else if (type == EConstraintParamNames::upp_lim_ang)
		*trg = src->upp_lim_ang;
	else if (type == EConstraintParamNames::upp_lim_lin)
		*trg = src->upp_lim_lin;
	else
		return false;
	return true;
}

bool ExConvert::getParams(FExConstraintParams* src, EConstraintParamNames type, ExSimPhyzHelpers::Constraint* trg)
{
	if (type == EConstraintParamNames::constraint_t)
		*trg = src->constr_type;
	else
		return false;
	return true;
}

bool ExConvert::getParams(FExConstraintParams* src, EConstraintParamNames type, float* trg)
{
	if (type == EConstraintParamNames::impulse_clamp)
		*trg = src->impulse_clamp;
	else if (type == EConstraintParamNames::tau)
		*trg = src->tau;
	else
		return false;
	return true;
}

bool ExConvert::getParams(FExConstraintParams* src, EConstraintParamNames type, int* trg)
{
	if (type == EConstraintParamNames::enables_spring)
		*trg = src->enables_spring;
	else
		return false;
	return true;
}
