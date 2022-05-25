#pragma once
#include "exactoSim/DataTypes/ExComplexParams.h"
#include "exactoSim/DataTypes/ExComponentParams.h"
#include "exactoSim/DataTypes/FExConstraintParams.h"

#define EC_DEF_FALSE TEXT("False")
#define EC_DEF_TRUE TEXT("True")
class ExConvert
{
public:
	static TArray<FString> getArrayBool(int32 value);
	static TArray<FString> getArrayString(FString value);
	static FString getStrFromVec(FVector vec);
	static FString getStrFromRot(FRotator rot);
	static FString getStrFromFloat(float val);
	static FString getBoolStrFromInt(uint8_t val, int last = 10);

	static bool getIntFromBoolStr(FString str, uint8_t * out);
	static bool getBoolArrayFromInt( uint8_t val, TArray<bool> * out, int last = 10);

	static FVector getVecFromStr(FString str);
	static bool checkVecStr(const FString str);
	static float getFloatFromStr(FString str);
	static int32 getIntFromStr(FString str);
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
	static bool updateParams(FExComponentParams * trg, EnExComponentParamNames type, int val);

	static bool getParams(FExComponentParams * src, EnExComponentParamNames type, FString * trg);
	static bool getParams(FExComponentParams * src, EnExComponentParamNames type, FVector * trg);
	static bool getParams(FExComponentParams * src, EnExComponentParamNames type, FRotator * trg);
	static bool getParams(FExComponentParams * src, EnExComponentParamNames type, float * trg);
	static bool getParams(FExComponentParams * src, EnExComponentParamNames type, int * trg);

	static bool updateParams(FExComplexParams * trg, EnExComplexParamNames type, FString val){return trg->update(type, val);}
	static bool updateParams(FExComplexParams * trg, EnExComplexParamNames type, FVector val){return trg->update(type, val);}
	static bool updateParams(FExComplexParams * trg, EnExComplexParamNames type, FRotator val){return trg->update(type, val);}
	static bool updateParams(FExComplexParams * trg, EnExComplexParamNames type, float val){return trg->update(type, val);}
	static bool updateParams(FExComplexParams * trg, EnExComplexParamNames type, int32 val){return trg->update(type, val);}

	static bool getParams(FExComplexParams * src, EnExComplexParamNames type, FString * trg){return src->copy(type,trg);}
	static bool getParams(FExComplexParams * src, EnExComplexParamNames type, FVector * trg){return src->copy(type,trg);}
	static bool getParams(FExComplexParams * src, EnExComplexParamNames type, FRotator * trg){return src->copy(type,trg);}
	static bool getParams(FExComplexParams * src, EnExComplexParamNames type, float * trg){return src->copy(type,trg);}
	static bool getParams(FExComplexParams * src, EnExComplexParamNames type, int32 * trg){return src->copy(type,trg);}
};


