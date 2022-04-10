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

float ExConvert::getFloatFromStr(FString str)
{
	return FCString::Atof(*str);
}
