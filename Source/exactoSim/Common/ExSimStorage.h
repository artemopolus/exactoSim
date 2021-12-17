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
	};
	struct es_component
	{
		FString name;
		AActor * target;
		btRigidBody * body;
		TArray<es_constraint_pair*> constraints; 
		es_complex * basis;	
	};
	struct es_complex
	{
		FString name;
		es_component * basis;
		TArray<es_component *> components;
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

	void setSceneObjName(FString name, FString type_name);

	void addSceneObjLocRot(FVector loc, FRotator rot);

	FVector2D clicked();

	FString switchMode ();

	int getMode();

	void manipulateGenerator(FVector loc, FRotator rot);

	bool touchActor(AActor * actor, FString & output);

	void pickActor(AActor * actor, FVector location);

	void moveActor(FVector location);

	void letActor();
	
};
