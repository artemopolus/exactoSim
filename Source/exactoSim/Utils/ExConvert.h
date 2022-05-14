#pragma once
#include "exactoSim/DataTypes/ExComponentParams.h"
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
	static FRotator getRotFromStr(FString str);

	static bool updateParams(FExConstraintParams * trg, EnExConstraintParamNames type, FString val);
	static bool updateParams(FExConstraintParams * trg, EnExConstraintParamNames type, FVector val);
	static bool updateParams(FExConstraintParams * trg, EnExConstraintParamNames type, ExSimPhyzHelpers::Constraint val);
	static bool updateParams(FExConstraintParams * trg, EnExConstraintParamNames type, float val);
	static bool updateParams(FExConstraintParams * trg, EnExConstraintParamNames type, int val);


	static bool getParams(FExConstraintParams * src, EnExConstraintParamNames type, FString * trg);
	static bool getParams(FExConstraintParams * src, EnExConstraintParamNames type, FVector * trg);
	static bool getParams(FExConstraintParams * src, EnExConstraintParamNames type, ExSimPhyzHelpers::Constraint * trg);
	static bool getParams(FExConstraintParams * src, EnExConstraintParamNames type, float * trg);
	static bool getParams(FExConstraintParams * src, EnExConstraintParamNames type, int * trg);
	
	static bool updateParams(FExComponentParams * trg, EnExComponentParamNames type, FString val);
	static bool updateParams(FExComponentParams * trg, EnExComponentParamNames type, FVector val);	
	static bool updateParams(FExComponentParams * trg, EnExComponentParamNames type, FRotator val);	
	static bool updateParams(FExComponentParams * trg, EnExComponentParamNames type, float val);
};


