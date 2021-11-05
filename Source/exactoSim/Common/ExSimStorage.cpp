// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimStorage.h"
#include <string>

// Sets default values
AExSimStorage::AExSimStorage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_SMPL, std::string("Simple"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_SPHERE,std::string ("Sphere"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_ROCK_SMALL,std::string ("Rock_S"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_ROCK_BIG,std::string ("Rock_B"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_SHOE,std::string ("Shoe"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_TREE_STICK, std::string("Stick"));

	
}

// Called when the game starts or when spawned
void AExSimStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExSimStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExSimStorage::updateDebugLog(FText text)
{
	UE_LOG(LogTemp, Warning,TEXT("%s"),*text.ToString());
}

void AExSimStorage::registerCmdToSelected(int type, float value)
{
	if (CurrentScene)
	{
		CurrentScene->sendCmdToSelected(type,value);
	}
}

void AExSimStorage::registerExtendedCmd(int type, int value)
{
	if (CurrentScene)
	{
		AExScene::actor_cmd cmd;
		cmd.value_int = type;
		cmd.value_float = 0;
		std::string * str = nullptr;
		switch (type)
		{
		case EXCT_SWITCH:
			str = GenObjType.Find(value);
			if (str != nullptr)
			{
				UE_LOG(LogTemp, Warning,TEXT("Find[%d]:%s"), value, *FString(str->c_str()));
				cmd.value_str = *str;
				CurrentScene->sendExtendedCmdToSelected(cmd);
			}
			break;
		default:
			break;
		}
	}
}


