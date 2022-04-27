#include "ExConvert.h"

FString ExConvert::getStrFromVec(FVector vec)
{
	return FString::SanitizeFloat(vec.X) + TEXT(" ;") + FString::SanitizeFloat(vec.Y) + TEXT(" ;") + FString::SanitizeFloat(vec.Z);
}

FString ExConvert::getStrFromFloat(float val)
{
	return FString::SanitizeFloat(val);
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
	return true;
}
