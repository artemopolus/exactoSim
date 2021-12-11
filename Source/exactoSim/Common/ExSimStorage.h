// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
private:
	UUserWidget * CurrentWidget = nullptr;

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

	TMap<int, std::string> GenObjType;
	TMap<int, std::string> ConstrType;

	void setTargetWidget( UUserWidget * widget);

	FVector2D clicked();
	
};
