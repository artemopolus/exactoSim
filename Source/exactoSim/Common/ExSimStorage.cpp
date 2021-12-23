// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimStorage.h"
#include "exactoSim/BulletSpec/BulletMinimal.h"
#include <string>

#include "Blueprint/UserWidget.h"
#include "exactoSim/Scene/ExSmplBox.h"
#include "exactoSim/UI/ExSimPlayer.h"

// Sets default values
AExSimStorage::AExSimStorage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_SMPL,			std::string ("Simple"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_SPHERE,			std::string ("Sphere"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_ROCK_SMALL,		std::string ("Rock_S"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_ROCK_BIG,		std::string ("Rock_B"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_SHOE,			std::string ("Shoe"));
	GenObjType.Add(AExSimStorage::exsim_genobj_type::EXGT_TREE_STICK,		std::string ("Stick"));

	ConstrType.Add(BulletHelpers::Constr::HINGE,			std::string("Hinge"));
	ConstrType.Add(BulletHelpers::Constr::HINGE2,			std::string("Hinge2"));
	ConstrType.Add(BulletHelpers::Constr::GEN6DOF_SPRING,	std::string("Gen6DOF_Spring"));
	
	ModeList.Add(es_modes::EDIT,FString("Edit"));
	ModeList.Add(es_modes::MOVE, FString("Move"));
	
	TargetName = "Test";
	TargetType = "Simple";
	TargetLocation = FVector(0,0,0);
	TargetRotation = FRotator(0,0,0);
	SceneObjCreated = 0;

	//создаем хранилище для новых объектов
	ExSimComplexList.Empty();
	es_complex * complex = new es_complex();
	complex->basis = nullptr;
	complex->name = "FreeObjects";
	ExSimComplexList.Add(complex);	
}

// Called when the game starts or when spawned
void AExSimStorage::BeginPlay()
{
	Super::BeginPlay();

	createTest("HelloWorld");
	
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

void AExSimStorage::setTargetWidget(UUserWidget* widget)
{
	CurrentWidget = widget;
}

void AExSimStorage::createTest(FString name, float mass, FVector loc, FRotator rot)
{
	const FString path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Simple.BP_ExSmplBox_Simple_C'";
	
	createSceneObj(name, path, mass, loc, rot, false);	
}

void AExSimStorage::createSceneObj()
{
	//SceneObjCreated++;
	if (ExWorld && ExWorld->ExFileManager)
	{
		FString path = ExWorld->ExFileManager->getPathToBlueprint(TargetType);
		FString name = TargetName + TEXT("_") + TargetType + TEXT("_") + FString::FromInt(SceneObjCreated);
		/*btRigidBody * body = nullptr;
		if (CurrentScene->addObjByPath(path, name, &body))
		{
			if (!body)
				return;
			AExSmplBox * target = static_cast<AExSmplBox*>(body->getUserPointer());
			es_component * component = new es_component();
			target->setEScomponent(component);
			component->body = body;
			component->target = target;
			es_complex * complex = ExSimComplexList[0]; //for free component
			component->basis = complex;
			complex->components.Add(component);
		}*/
		createSceneObj(name, path, 1.0f, FVector(0,0,0), FRotator(0,0,0), true);
	}
}

void AExSimStorage::createSceneObj(FString name, FString path, float mass, FVector loc, FRotator rot, bool use_genloc)
{
	SceneObjCreated++;
	if (CurrentScene)
	{
		btRigidBody * body = nullptr;
		if (CurrentScene->addObjByPath(path, name, &body, mass, loc, rot, use_genloc))
		{
			if (!body)
				return;
			AExSmplBox * target = static_cast<AExSmplBox*>(body->getUserPointer());
			es_component * component = new es_component();
			target->setEScomponent(component);
			component->body = body;
			component->target = target;
			component->name = name;
			es_complex * complex = ExSimComplexList[0]; //for free component
			component->basis = complex;
			complex->components.Add(component);			
		}
	}
}

void AExSimStorage::createConstraint(AActor* target, AActor* parent, AExactoPhysics::es_constraint params)
{
	AExSmplBox * parent_actor = static_cast<AExSmplBox*>(parent);
	es_component * parent_component = parent_actor->getEScomponent();
	if (parent_component->basis == ExSimComplexList[0])
	{
		parent_component->basis = new es_complex();
		parent_component->basis->name = "Default";
		ExSimComplexList.Add(parent_component->basis);
		ExSimComplexList[0]->components.Remove(parent_component);
	}
	AExSmplBox * target_actor = static_cast<AExSmplBox*>(target);
	es_component * target_component = target_actor->getEScomponent();
	
	target_component->basis->components.Remove(target_component);
	
	target_component->basis = parent_component->basis;
	parent_component->basis->components.Add(target_component);
}

void AExSimStorage::setSceneObjName(FString name, FString type_name)
{
	TargetName = name;
	TargetType = type_name;
}

void AExSimStorage::addSceneObjLocRot(FVector loc, FRotator rot)
{
	TargetLocation += loc;
	TargetRotation += rot;
	CurrentScene->moveGenerator(loc, rot);
}

FVector2D AExSimStorage::clicked()
{
	
	FString output = TEXT("Mouse pos: clicked") ;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, output);
	return FVector2D(0,0);
}

FString AExSimStorage::switchMode()
{
	CurrentMode ++;
	if (CurrentMode == es_modes::END)
	{
		CurrentMode = 0;
	}
	return *ModeList.Find(CurrentMode);
}

int AExSimStorage::getMode()
{
	return CurrentMode;
}

void AExSimStorage::manipulateGenerator(FVector loc, FRotator rot)
{
	if (CurrentScene)
	{
		CurrentScene->moveGenerator(loc, rot);
	}
}

bool AExSimStorage::touchActor(AActor* actor, FString & output)
{
	if (!CurrentScene)
		return false;
	for (int i = 0; i < actor->Tags.Num(); i++)
	{
		if (actor->Tags[i].ToString() == CurrentScene->BaseTag)
		{
			AExSmplBox * target = static_cast<AExSmplBox*>(actor);
			if (!target->getEScomponent())
				return false;
			output += TEXT("Component name: ") + target->getEScomponent()->target->GetName() + TEXT("\n");
			output += TEXT("Basis: ") + target->getEScomponent()->basis->name + TEXT("\n");
			output += TEXT("Constraints:\n");
			for (const auto elem : target->getEScomponent()->constraints)
			{
				output += FString(ConstrType.Find(elem->type)->c_str()) + TEXT("\n");
			}
			return true;
		}
	}
	return false;
}

void AExSimStorage::pickActor(AActor* actor, FVector location)
{
	if (actor&&CurrentScene)
	{
		AExSmplBox * target = static_cast<AExSmplBox*>(actor);
		btRigidBody * body = target->getEScomponent()->body;
		CurrentScene->pickTrgBody(body, location);
	}
}

void AExSimStorage::moveActor(FVector location)
{
	if(CurrentScene)
	{
		CurrentScene->moveTrgBody(location);
	}
}

void AExSimStorage::letActor()
{
	if(CurrentScene)
	{
		CurrentScene->letTrgBody();
	}
}


