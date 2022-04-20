// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "exactoSim/exactoWorld.h"
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
	struct es_complex;
	struct es_component;
	struct es_constraint_pair
	{
		FString name;
		es_component * parent;
		btTypedConstraint *constraint;
		BulletHelpers::Constr type;
		AExactoPhysics::es_constraint * params;
	};
	struct es_component
	{
		FString Name;
		FString Path;
		AActor * Target;
		btRigidBody * Body;
		TArray<es_constraint_pair*> Constraints; 
		es_complex * Basis;
		es_component()
		{
			Target = nullptr;
			Body = nullptr;
			Basis = nullptr;
		}
		bool getConstraintNames(TArray<FString> * names);
		bool addConstraint(FString name);
		bool removeConstraint(FString name);
		FString getName();
		void setName(FString name);
	};
	struct es_complex
	{
		FString name;
		es_component * basis;
		TArray<es_component *> components;
		es_complex()
		{
			name = "Default";
			basis = nullptr;
		}
		
	};
	TArray<es_complex*> ExSimComplexList;
	TMap<int, std::string> GenObjType;
	TMap<int, std::string> ConstrType;
	enum es_modes
	{
		EDIT = 0,
		MOVE,
		END
	};
	TMap<AExactoPhysics::es_options_list, FString> OptionNamesPtr;
	TMap<FString, FString> OptionValuePairsPtr;
	
private:
	int CurrentMode = es_modes::EDIT;
	TMap<int, FString> ModeList;
	UUserWidget * CurrentWidget = nullptr;
	uint32 SceneObjCreated = 0;

	FString TargetName;
	FString TargetType;

	FVector TargetLocation;
	FRotator TargetRotation;


	
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

	void registerExtendedCmd(int type, int value);

	void setTargetWidget( UUserWidget * widget);

	UFUNCTION(BlueprintCallable)
		void createTest(FString name, float mass = 1.0f, FVector loc = FVector(0,0,50), FRotator rot = FRotator(0,0,0));

	void createSceneObj(void);
	void createSceneObj(FString name, FString path, float mass = 1.0f, FVector loc = FVector(0,0,0), FRotator rot = FRotator(0,0,0), bool use_genloc = true);
	void createConstraint(AActor * target, AActor * parent, AExactoPhysics::es_constraint params);
	void createConstraint(AActor * target, AExactoPhysics::es_constraint * params);
	bool getConstraint(const AActor * target, TArray<es_constraint_pair *> * constr);

	void setSceneObjName(FString name, FString type_name);

	void addSceneObjLocRot(FVector loc, FRotator rot);

	FVector2D clicked();

	FString switchMode ();

	int getMode();

	void manipulateGenerator(FVector loc, FRotator rot);

	bool touchActor(AActor * actor, FString & output);
	es_component * getExSmComponent(AActor * actor);
	void pickActor(AActor * actor, FVector location);
	void moveActor(FVector location);
	void letActor();
	bool getActorInfo(FVector & pos);

	void saveExSimComplex(es_complex * target);
	
	void convertExSimComplex(es_complex * target, const AExSimFileManager::es_complex_params * src);

	void saveExSimComplex(int index);

	void loadExSimComplex();

	void resetOptVPP();
	void setOptVPP(es_constraint_pair * params);
	void setOptVPP(AActor * actor, FString constraint_name);

	

private:
	void createComplex(es_component * component, FString new_complex_name);
	
};
