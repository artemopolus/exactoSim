#pragma once

class ExConvert
{
public:
	static FString getStrFromVec(FVector vec);
	static FString getStrFromFloat(float val);

	static FVector getVecFromStr(FString str);
	static bool checkVecStr(const FString str);
	static float getFloatFromStr(FString str);
	
};
