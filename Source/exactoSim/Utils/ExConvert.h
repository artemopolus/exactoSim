#pragma once
#include "exactoSim/DataTypes/FExConstraintParams.h"

class ExConvert
{
public:
	static FString getStrFromVec(FVector vec);
	static FString getStrFromRot(FRotator rot);
	static FString getStrFromFloat(float val);
	static FString getBoolStrFromInt(uint8_t val, int last = 10);

	static bool getIntFromBoolStr(FString str, uint8_t * out);
	static bool getBoolArrayFromInt( uint8_t val, TArray<bool> * out, int last = 10);

	static FVector getVecFromStr(FString str);
	static bool checkVecStr(const FString str);
	static float getFloatFromStr(FString str);

	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, FString val);
	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, FVector val);
	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, ExSimPhyzHelpers::Constraint val);
	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, float val);
	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, int val);

	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, FString * trg);
	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, FVector * trg);
	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, ExSimPhyzHelpers::Constraint * trg);
	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, float * trg);
	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, int * trg);
	
	
};
