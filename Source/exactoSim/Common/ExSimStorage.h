// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExCommander.h"
#include "exactoSim/exactoWorld.h"
#include "exactoSim/DataTypes/ExFactoryOperator.h"
#include "exactoSim/DataTypes/ExSimComponent.h"
#include "exactoSim/Scene/ExScene.h"
#include "GameFramework/Actor.h"
#include "ExSimStorage.generated.h"

UCLASS()
class EXACTOSIM_API AExSimStorage : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExSimStorage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExScene * CurrentScene;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExactoWorld * ExWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="For test")
		FVector PivotP = FVector(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="For test")
	FVector PivotT = FVector(0,10,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="For test")
	FVector LowLimLin = FVector(0, -5, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="For test")
	FVector UppLimLin = FVector(0,5,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="For test")
	float Stiffness = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="For test")
	float Dumping = 0.01f;

	enum exsim_genobj_type
	{
		EXGT_SMPL = 0,
		EXGT_SPHERE,
		EXGT_ROCK_SMALL,
		EXGT_ROCK_BIG,
		EXGT_TREE_STICK,
		EXGT_SHOE,

		EXGT_END
	};
	enum exsim_cmd_type
	{
		EXCT_CREATE = 0,
		EXCT_MOVE,
		EXCT_ROTATE,
		EXCT_DELETE,
		EXCT_SWITCH
	};

	struct ParamHolder
	{
		FString Name;
		int ParamType;
		TArray<FString> Value;
		EnExParamEdit EditType;
		ParamHolder(FString name, EnExConstraintParamNames paramtype, TArray<FString> value, EnExParamEdit edittype):
			Name(name), ParamType(static_cast<int32>(paramtype)), Value(value), EditType(edittype)
		{}
		ParamHolder(FString name, EnExComponentParamNames paramtype, TArray<FString> value, EnExParamEdit edittype):
			Name(name), ParamType(static_cast<int32>(paramtype)), Value(value), EditType(edittype)
		{}
		ParamHolder(FString name, EnExComplexParamNames paramtype, TArray<FString> value, EnExParamEdit edittype):
			Name(name), ParamType(static_cast<int32>(paramtype)), Value(value), EditType(edittype)
		{}
	};
	
	TArray<ExSimComplex*> ExSimComplexList;
	TMap<int, std::string> GenObjType;
	TMap<int, std::string> ConstrType;
	enum es_modes
	{
		EDIT = 0,
		MOVE,
		END
	};
	TMap<EnExConstraintParamNames, FString> OptionNamesPtr;
	TMap<EnExConstraintParamNames, FString> OptionValuesPtr;
	TMap<FString, FString> OptionValuePairsPtr;

	// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FConstraintChanged, int, type, FExCommonParams*, value);
	// FConstraintChanged EssEvOnConstraintChanged;
private:
	int CurrentMode = es_modes::EDIT;
	TMap<int, FString> ModeList;
	UUserWidget * CurrentWidget = nullptr;
	uint32 SceneObjCreated = 0;

	FString TargetName;
	FString TargetType;

	FVector TargetLocation;
	FRotator TargetRotation;
	
	ExCommander * CoCoCoProvider;
	ExSimObject * CurrentConstraintPtr = nullptr;
	ExSimComplex * CurrentComplex = nullptr;

	ExFactoryOperator * ParamsOperator = nullptr;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void updateDebugLog(FText text);

	UFUNCTION(BlueprintCallable)
		void registerCmdToSelected(int type, float value);


	void setTargetWidget( UUserWidget * widget);

	UFUNCTION(BlueprintCallable)
		void createTest(FString name, float mass = 1.0f, FVector loc = FVector(0,0,50), FRotator rot = FRotator(0,0,0));

	void onCommandRegistered(FExCommonParams* params);

	void createComponent(FString name, FString path, float mass = 1.0f, FVector loc = FVector(0, 0, 0),
	                     FRotator rot = FRotator(0, 0, 0), bool use_genloc = true);
	void createConstraint(ExSimComponent* target, ExSimComponent* parent);

	void createConstraint( FExConstraintParams * params);
	void createConstraint( ExSimConstraintPair * pair);
	void activateConstraint(FExConstraintParams * param);
	void deactivateConstraint( FExConstraintParams * param);
	void deleteConstraint( FExConstraintParams * params);

	void createComponent(FExComponentParams * component);
	void createComplex(FExComplexParams * complex);

	void hideComponent(FExComponentParams * params);
	void showComponent(FExComponentParams * params);
	void deleteComponent(FExComponentParams * params);
	void deleteComplex(FExComplexParams * params);
	
	void updateConstraint();
	void update(ExSimConstraintPair * pair);
	void update(ExSimComponent * component);
	void update(ExSimComplex * complex);
	

	void setSceneObjName(FString name, FString type_name);

	void addSceneObjLocRot(FVector loc, FRotator rot);

	FVector2D clicked();

	FString switchMode ();

	int getMode();

	void manipulateGenerator(FVector loc, FRotator rot);

	bool touchActor(AActor * actor, FString & output);
	ExSimComponent * getExSmComponent(AActor * actor);
	void pickActor(AActor * actor, FVector location);
	void moveActor(FVector location);
	void letActor();
	bool getActorInfo(FVector & pos);

	void selectComplex(ExSimComponent * trg);

	void loadComplex();
	
	void saveComplex();
	void saveComplex(int index);
	void saveComplex(ExSimComplex * target);
	


	void loadExSimComplex();

	void createNewConstraint();
	void updateOptVPP();
	void setOptVPP(ExSimConstraintPair * params);

	template<typename Tparams, typename Tnames, typename Tdict>
	void updateHolderByTemplate(Tparams * params, TArray<ParamHolder> * holder);
	
	
	void initComplexCommand(TArray<ParamHolder> * holder);
	void initComponentCommand(TArray<ParamHolder> * holder);
	void initConstraintCommand(TArray<ParamHolder> * holder);

	void selectCommand(ExSimObject* trg, TArray<ParamHolder> * holder);

	void setConstraintParentCommand(ExSimComponent * comp);
	void setConstraintTargetCommand(ExSimComponent * comp);

	void createCommand();
	
	void deleteCommand();
	
	void updateCommand(int type, FString str);
	void updateCommand(EnExComplexParamNames type, FString str);
	void updateCommand(EnExComponentParamNames type, FString str);
	void updateCommand(EnExConstraintParamNames type, FString str);
	void updateCommand(FExCommonParams * params, TArray<ParamHolder> * holder);
	void updateCommand(FExComponentParams * params, TArray<ParamHolder> * holder);
	void updateCommand(FExComplexParams * params, TArray<ParamHolder> * holder);
	void updateCommand(FExConstraintParams * params, TArray<ParamHolder> * holder);
	void undoCommand();

	ExCommander * getCommander() const {return  CoCoCoProvider;}


private:
	
	void createComplex(ExSimComponent * component, FString new_complex_name);
	void createComplex(ExSimComplex * complex);
	void createComponent(ExSimComponent * component);
	void createConstraintPair(ExSimConstraintPair * pair);
	void createTestObjects();
	void createTestObjects2();

	
};

template <typename Tparams, typename Tnames, typename Tdict>
void AExSimStorage::updateHolderByTemplate(Tparams* params, TArray<ParamHolder>* holder)
{
	TMap<Tnames, FString> tmp_names;
	TMap<Tnames, TArray<FString>> tmp_values;
	TMap<Tnames, EnExParamEdit> tmp_edit;
	Tdict::getInitValues(&tmp_values, params);
	Tdict::getDefaultNames(&tmp_names);
	Tdict::getEditTypeCreate(&tmp_edit);
	for (const auto & value : tmp_values)
	{
		const auto key = value.Key;
		const auto name = * tmp_names.Find(key);
		holder->Add(ParamHolder(name, key, value.Value, *tmp_edit.Find(key)));
	}
}


