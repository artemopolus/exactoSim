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
