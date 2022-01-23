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
	ConstrType.Add(BulletHelpers::Constr::P2P, std::string("P2P"));
	ConstrType.Add(BulletHelpers::Constr::NONE, std::string(""));
	
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

	const FString vector_str("0.0; 0.0; 0.0");
	const FString value_str("0.0");
	const FString name_str("default");
	
	OptionNamesPtr.Add( AExSimStorage::es_options_list::parent_pivot, "Parent Pivot");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::target_pivot, "Target Pivot");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::low_lim_lin, "Low Lim Lin");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::upp_lim_lin, "Upp Lim Lin");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::low_lim_ang, "Low Lim Ang");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::upp_lim_ang, "Upp Lim Ang");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::en_spring, "Enable Spring");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::stiff_lin, "Stiff Lin");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::dump_lin, "Dump Lin");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::parent_name, "Parent name");
    OptionNamesPtr.Add(AExSimStorage::es_options_list::target_name, "Target name");
    
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::parent_pivot], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::target_pivot], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::low_lim_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::upp_lim_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::low_lim_ang], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::upp_lim_ang], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::stiff_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::dump_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::en_spring], "0; 0; 0; 0; 0; 0");
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::parent_name], name_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[AExSimStorage::es_options_list::target_name], name_str);
	
	
}

// Called when the game starts or when spawned
void AExSimStorage::BeginPlay()
{
	Super::BeginPlay();

	//createTest("HelloWorld");
	FString path = "Class'/Game/Blueprint/Scene/BP_ESB_Magnet.BP_ESB_Magnet_C'";
	const FString magnet_name = "Magnet";
	const FVector magnet_pos(0,0,200);
	const FVector magnet_relpivot0(0,0,20);
	const FVector magnet_relpivot1(20,0,20);
	createSceneObj(magnet_name, path, 100.0f, magnet_pos, FRotator(0,0,0), false);	
	path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Simple.BP_ExSmplBox_Simple_C'";	
	createSceneObj("HelloWorld", path, 1.0f, FVector(0,0,50), FRotator(0,0,0), false);
	path = "Class'/Game/Blueprint/Scene/BP_ESB_Spring.BP_ESB_Spring_C'";
	const FString spring_name = "Spring";
	createSceneObj(spring_name, path, 1.0f, FVector(0,50,250), FRotator(0,0,0), false);

	es_component * target = nullptr;

	for (auto & system : ExSimComplexList)
	{
		for (auto & component : system->components)
		{
			if (component->name == magnet_name)
			{
				es_constraint_pair * p = new es_constraint_pair();
				p->constraint = CurrentScene->fixP2PBody(component->body, magnet_relpivot0);
				p->type = BulletHelpers::Constr::P2P;
				p->name = magnet_name;
				p->parent = nullptr;
				AExactoPhysics::es_constraint *pp = new AExactoPhysics::es_constraint();
				pp->pivot_p = magnet_relpivot0;
				p->params = pp;
				component->constraints.Add(p);

				es_constraint_pair * p1 = new es_constraint_pair();
				p->constraint = CurrentScene->fixP2PBody(component->body, magnet_relpivot1);
				p->type = BulletHelpers::Constr::P2P;
				p->name = magnet_name + TEXT("_P2P1");
				p->parent = nullptr;
				AExactoPhysics::es_constraint *pp1 = new AExactoPhysics::es_constraint();
				pp1->pivot_p = magnet_relpivot1;
				p1->params = pp1;
				component->constraints.Add(p1);
				
				target = component;
				break;
			}
		}
	}
	AExactoPhysics::es_constraint *params = new AExactoPhysics::es_constraint();
	params->pivot_p = PivotP;
	params->pivot_t = PivotT;

	params->low_lim_lin = LowLimLin;
	params->upp_lim_lin = UppLimLin;

	params->low_lim_ang = FVector::ZeroVector;
	params->upp_lim_ang = FVector::ZeroVector;

	params->en_spring[0] = 1;
	params->en_spring[1] = 1;
	params->en_spring[2] = 1;

	float stiff = Stiffness;
	float dump = Dumping;
	params->stiff_lin = FVector(stiff, stiff, stiff);
	params->dump_lin = FVector(dump, dump, dump);
	
	if (target)
	{
		createComplex(target, magnet_name);
		es_component * spring = nullptr;
		for (auto & component : ExSimComplexList[0]->components)
		{
			if (component->name == spring_name)
			{
				spring = component;

			}
		}

		if (spring)
		{
				es_constraint_pair * p = new es_constraint_pair();
				p->constraint = CurrentScene->fixP2PBody(spring->body, FVector(0,0,20));
				p->type = BulletHelpers::Constr::P2P;
				p->name = magnet_name;
				p->parent = nullptr;
				AExactoPhysics::es_constraint * fix_params = new AExactoPhysics::es_constraint();
				fix_params->pivot_p = FVector(0,0,20);
				p->params = fix_params;
				spring->constraints.Add(p);

				es_constraint_pair * gen = new es_constraint_pair();
				gen->constraint = CurrentScene->fixGen6dofSpring(target->body, spring->body, *params);
				gen->type = BulletHelpers::Constr::GEN6DOF_SPRING;
				gen->name = "Spring Imitator";
				gen->parent = target;

				gen->params = params;

				spring->constraints.Add(gen);

				spring->basis->components.Remove(spring);
				spring->basis = target->basis;
				target->basis->components.Add(spring);			
		}
		
	}
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
			component->path = path;
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

void AExSimStorage::createConstraint(AActor* target, AExactoPhysics::es_constraint * params)
{
	if (params->constr_type != BulletHelpers::Constr::P2P)
		return;
	AExSmplBox * actor = static_cast<AExSmplBox*>(target);
	es_component * component = actor->getEScomponent();
	if (component->basis == ExSimComplexList[0])
		createComplex(component, component->name + TEXT("_Complex"));

	es_constraint_pair * p = new es_constraint_pair();
    p->constraint = CurrentScene->fixP2PBody(component->body, params->pivot_p);
    p->type = BulletHelpers::Constr::P2P;
    p->name = params->name_p;
	p->parent = nullptr;
	p->params = params;
    component->constraints.Add(p);
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

bool AExSimStorage::getActorInfo(FVector& pos)
{
	if (CurrentScene)
	{
		AActor * actor = nullptr;
		if (CurrentScene->getTrgBody(&actor))
		{
			pos = actor->GetActorLocation();
			return true;
		}
	}
	return false;
}

void AExSimStorage::saveExSimComplex(es_complex* target)
{
	AExSimFileManager::es_complex_params * cmplx = new AExSimFileManager::es_complex_params();
	cmplx->string_list.Add("Name",target->name);
	cmplx->string_list.Add("BasisName",target->basis->name);
	for (const auto component : target->components)
	{
		AExSimFileManager::es_component_params * cmpnt = new AExSimFileManager::es_component_params();
		cmpnt->string_list.Add("Name",component->name);
		cmpnt->string_list.Add("Path",component->path);
		for (const auto constr : component->constraints)
		{
			AExSimFileManager::es_constraint_params * p = new AExSimFileManager::es_constraint_params();
			p->string_list.Add("Name", constr->name);
			p->string_list.Add("Type",FString(ConstrType[constr->type].c_str()));
			p->string_list.Add("Parent", (constr->parent) ? constr->parent->name : "NULL");
			p->string_list.Add("NameParent",constr->params->name_p);
			p->string_list.Add("NameTarget",constr->params->name_t);
			
			p->vector_list.Add("AxisParent",constr->params->axis_p);
			p->vector_list.Add("AxisTarget",constr->params->axis_t);
			p->vector_list.Add("DumpAngular",constr->params->dump_ang);
			p->vector_list.Add("DumpLinear",constr->params->dump_lin);
			p->vector_list.Add("LowerLimAngular",constr->params->low_lim_ang);
			p->vector_list.Add("UpperLimAngular",constr->params->upp_lim_ang);
			p->vector_list.Add("PivotParent",constr->params->pivot_p);
			p->vector_list.Add("PivotTarget",constr->params->pivot_t);
			p->vector_list.Add("StiffnessAngular",constr->params->stiff_ang);
			p->vector_list.Add("StiffnessLinear",constr->params->stiff_lin);
			p->vector_list.Add("LowerLimLinear",constr->params->low_lim_lin);
			p->vector_list.Add("UpperLimLinear",constr->params->upp_lim_lin);
			cmpnt->constraints.Add(p);
		}
		cmplx->components.Add(cmpnt);
	}
	ExWorld->ExFileManager->saveEsComplexParams(cmplx);

}

void AExSimStorage::convertExSimComplex(es_complex* target, const AExSimFileManager::es_complex_params* src)
{
	target->name = src->string_list.FindRef("Name");
	const FString basis_name = src->string_list.FindRef("BasicName");
	for (const auto & component : src->components)
	{
		es_component * c = new es_component();
		c->name = component->string_list.FindRef("Name");
		if (c->name == basis_name)
			target->basis = c;
		c->path = component->string_list.FindRef("Path");
		c->basis = target;
		for (const auto & constraint : component->constraints)
		{
			es_constraint_pair * p = new es_constraint_pair();
			p->name = constraint->string_list.FindRef("Name");
			p->type = static_cast<BulletHelpers::Constr>(*ConstrType.FindKey(TCHAR_TO_UTF8(*constraint->string_list.FindRef("Type"))));
			p->params->name_p = constraint->string_list.FindRef("NameParent");
			
			p->params->axis_p = constraint->vector_list.FindRef("AxisParent");
			p->params->axis_t = constraint->vector_list.FindRef("AxisTarget");
			p->params->dump_ang = constraint->vector_list.FindRef("DumpAngular");
			p->params->dump_lin = constraint->vector_list.FindRef("DumpLinear");
			p->params->low_lim_ang = constraint->vector_list.FindRef("LowerLimAngular");
			p->params->upp_lim_ang = constraint->vector_list.FindRef("UpperLimAngular");
			p->params->pivot_p = constraint->vector_list.FindRef("PivotParent");
			p->params->pivot_t = constraint->vector_list.FindRef("PivotTarget");
			p->params->stiff_lin = constraint->vector_list.FindRef("StiffnessLinear");
			p->params->stiff_ang = constraint->vector_list.FindRef("StiffnessAngular");
			p->params->low_lim_lin = constraint->vector_list.FindRef("LowerLimLinear");
			p->params->upp_lim_lin = constraint->vector_list.FindRef("UpperLimLinear");
			c->constraints.Add(p);
			
		}
		target->components.Add(c);
	}
}

void AExSimStorage::saveExSimComplex(int index)
{
	saveExSimComplex(ExSimComplexList[index]);
}

void AExSimStorage::loadExSimComplex()
{
	TArray<AExSimFileManager::es_complex_params * > c_p_list;
	ExWorld->ExFileManager->loadEsComplexParams(c_p_list);
}

void AExSimStorage::createComplex(es_component* component, FString new_complex_name)
{
	es_complex * base = new es_complex();
	base->name = new_complex_name;
	base->basis = component;
	base->components.Add(component);
	es_complex * old = component->basis;
	ExSimComplexList.Add(base);
	component->basis = base;
	old->components.Remove(component);
	if (old != ExSimComplexList[0])
	{
		if (old->components.Num() == 0)
		{
			ExSimComplexList.Remove(old);
		}
	}
	
}




