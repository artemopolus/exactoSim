#pragma once
#include "exactoSim/DataTypes/FExConstraintParams.h"

class ExConvert
{
public:
	static FString getStrFromVec(FVector vec);
	static FString getStrFromFloat(float val);

	static FVector getVecFromStr(FString str);
	static bool checkVecStr(const FString str);
	static float getFloatFromStr(FString str);

	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, FString val);
	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, FVector val);
	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, BulletHelpers::Constr val);
	static bool updateParams(FExConstraintParams * trg, EConstraintParamNames type, float val);

	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, FString * trg);
	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, FVector * trg);
	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, BulletHelpers::Constr * trg);
	static bool getParams(FExConstraintParams * src, EConstraintParamNames type, float * trg);
	
};
