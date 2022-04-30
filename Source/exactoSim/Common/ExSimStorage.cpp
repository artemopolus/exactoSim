// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimStorage.h"
#include "exactoSim/Utils/ExConvert.h"
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

	ConstrType.Add(ExSimPhyzHelpers::Constraint::HINGE,			std::string("Hinge"));
	ConstrType.Add(ExSimPhyzHelpers::Constraint::HINGE2,			std::string("Hinge2"));
	ConstrType.Add(ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING,	std::string("Gen6DOF_Spring"));
	ConstrType.Add(ExSimPhyzHelpers::Constraint::P2P, std::string("P2P"));
	ConstrType.Add(ExSimPhyzHelpers::Constraint::NONE, std::string(""));
	
	ModeList.Add(es_modes::EDIT,FString("Edit"));
	ModeList.Add(es_modes::MOVE, FString("Move"));
	
	TargetName = "Test";
	TargetType = "Simple";
	TargetLocation = FVector(0,0,0);
	TargetRotation = FRotator(0,0,0);
	SceneObjCreated = 0;

	//создаем хранилище для новых объектов
	ExSimComplexList.Empty();
	ExSimComplex * complex = new ExSimComplex();
	// complex->basis = nullptr;
	complex->setName( "FreeObjects");
	ExSimComplexList.Add(complex);

	const FString vector_str("0.0; 0.0; 0.0");
	const FString value_str("0.0");
	const FString name_str("default");
	
	OptionNamesPtr.Add(EConstraintParamNames::parent_pivot, "Parent Pivot");
    OptionNamesPtr.Add(EConstraintParamNames::target_pivot, "Target Pivot");
    OptionNamesPtr.Add(EConstraintParamNames::low_lim_lin, "Low Lim Lin");
    OptionNamesPtr.Add(EConstraintParamNames::upp_lim_lin, "Upp Lim Lin");
    OptionNamesPtr.Add(EConstraintParamNames::low_lim_ang, "Low Lim Ang");
    OptionNamesPtr.Add(EConstraintParamNames::upp_lim_ang, "Upp Lim Ang");
    OptionNamesPtr.Add(EConstraintParamNames::en_spring, "Enable Spring");
    OptionNamesPtr.Add(EConstraintParamNames::stiff_lin, "Stiff Lin");
    OptionNamesPtr.Add(EConstraintParamNames::stiff_ang, "Stiff Ang");
    OptionNamesPtr.Add(EConstraintParamNames::dump_lin, "Dump Lin");
    OptionNamesPtr.Add(EConstraintParamNames::dump_ang, "Dump Ang");
    OptionNamesPtr.Add(EConstraintParamNames::parent_name, "Parent name");
    OptionNamesPtr.Add(EConstraintParamNames::target_name, "Target name");
	OptionNamesPtr.Add(EConstraintParamNames::constraint_t, "Constraint type");
	OptionNamesPtr.Add(EConstraintParamNames::constraint_name, "Constraint Name");



	resetOptVPP();
 
	
	
}




// Called when the game starts or when spawned
void AExSimStorage::BeginPlay()
{
	Super::BeginPlay();

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

	ExSimComponent * target = nullptr;

	for (auto & system : ExSimComplexList)
	{
		for (auto & component : *system->getComponents())
		{
			if (component->getName() == magnet_name)
			{
				
				FExConstraintParams *pp = new FExConstraintParams();
				pp->pivot_p = magnet_relpivot0;
				pp->axis_p = component->getTarget()->GetActorLocation();
				pp->name_p = component->getName();
				pp->constr_type = ExSimPhyzHelpers::Constraint::P2P;
				pp->name_constraint = magnet_name + TEXT("_1");
				pp->impulse_clamp = 30.f;
				pp->tau = 0.001f;
				ExSimConstraintPair * p = CurrentScene->fixP2P(component, pp);
				component->getConstraints()->Add(p);



				pp->name_constraint = magnet_name + TEXT("_2");
				pp->pivot_p = magnet_relpivot1;				
				ExSimConstraintPair *p1 = CurrentScene->fixP2P(component,pp);
				component->getConstraints()->Add(p1);
				
				target = component;
				break;
			}
		}
	}
	FExConstraintParams *params = new FExConstraintParams();
	params->pivot_p = PivotP;
	params->pivot_t = PivotT;

	params->low_lim_lin = LowLimLin;
	params->upp_lim_lin = UppLimLin;

	params->low_lim_ang = FVector::ZeroVector;
	params->upp_lim_ang = FVector::ZeroVector;


	float stiff = Stiffness;
	float dump = Dumping;
	params->stiff_lin = FVector(stiff, stiff, stiff);
	params->dump_lin = FVector(dump, dump, dump);
	
	if (target)
	{
		createComplex(target, magnet_name);
		ExSimComponent * spring = nullptr;
		for (auto & component : *ExSimComplexList[0]->getComponents())
		{
			if (component->getName() == spring_name)
				spring = component;
		}

		if (spring)
		{
			FExConstraintParams* fix_params = new FExConstraintParams();
			fix_params->pivot_p = FVector(0, 0, 20);
			fix_params->axis_p = spring->getTarget()->GetActorLocation();
			fix_params->name_p = spring->getName();
			fix_params->constr_type = ExSimPhyzHelpers::Constraint::P2P;
			fix_params->name_constraint = TEXT("P2P magnet");
			fix_params->impulse_clamp = 30.f;
			fix_params->tau = 0.001f;
			

			ExSimConstraintPair* p = CurrentScene->fixP2P(spring, fix_params);
			spring->getConstraints()->Add(p);


			params->enables_spring = 7;
			params->constr_type = ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING;
			params->name_constraint = TEXT("g6dof spring");
			ExSimConstraintPair* gen = CurrentScene->fixGen6dofSpring(target, spring, params);
			

			spring->getConstraints()->Add(gen);

			spring->getBasis()->getComponents()->Remove(spring);
			spring->setBasis(target->getBasis());
			target->getBasis()->getComponents()->Add(spring);
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
		createSceneObj(name, path, 1.0f, FVector(0,0,0), FRotator(0,0,0), true);
	}
}

void AExSimStorage::createSceneObj(FString name, FString path, float mass, FVector loc, FRotator rot, bool use_genloc)
{
	SceneObjCreated++;
	if (CurrentScene)
	{
		ExSimComponent* component = nullptr;
		if (CurrentScene->addObjByPath(&component, path, name, mass, loc, rot, use_genloc))
		{
			if (component != nullptr)
			{
				ExSimComplex* complex = ExSimComplexList[0]; //for free component
				component->setBasis(complex);
				complex->getComponents()->Add(component);
			}
		}
	}
}

void AExSimStorage::createConstraint(AActor* target, AActor* parent, FExConstraintParams params)
{
	AExSmplBox * parent_actor = static_cast<AExSmplBox*>(parent);
	ExSimComponent * parent_component = parent_actor->getEScomponent();
	if (parent_component->getBasis() == ExSimComplexList[0])
	{
		parent_component->setBasis( new ExSimComplex());
		parent_component->getBasis()->setName( "Default");
		ExSimComplexList.Add(parent_component->getBasis());
		ExSimComplexList[0]->getComponents()->Remove(parent_component);
	}
	AExSmplBox * target_actor = static_cast<AExSmplBox*>(target);
	ExSimComponent * target_component = target_actor->getEScomponent();
	
	target_component->getBasis()->getComponents()->Remove(target_component);
	
	target_component->setBasis( parent_component->getBasis());
	parent_component->getBasis()->getComponents()->Add(target_component);
}

void AExSimStorage::createConstraint(AActor* target, FExConstraintParams * params)
{
	if (params->constr_type != ExSimPhyzHelpers::Constraint::P2P)
		return;
	AExSmplBox * actor = static_cast<AExSmplBox*>(target);
	ExSimComponent * component = actor->getEScomponent();
	if (component->getBasis() == ExSimComplexList[0])
		createComplex(component, component->getName() + TEXT("_Complex"));

	ExSimConstraintPair * p = new ExSimConstraintPair();
    p->setConstraint( CurrentScene->fixP2P(component->getBody(), params));
	p->setType(ExSimPhyzHelpers::Constraint::P2P);
    p->setName( params->name_p);
	p->setParams( params);
    component->getConstraints()->Add(p);
}



bool AExSimStorage::getConstraint(const AActor* target, TArray<ExSimConstraintPair *> * constr )
{
	if (constr->Num()>0)
		constr->Empty();
	for (ExSimComplex * complex : ExSimComplexList)
	{
		for (ExSimComponent * component : *complex->getComponents())
		{
			if (target == component->getTarget())
			{
				for(ExSimConstraintPair * cp : *component->getConstraints())
					constr->Add(cp);
				return true;
			}
		}
	}
	return false;
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
			output += TEXT("Component name: ") + target->getEScomponent()->getTarget()->GetName() + TEXT("\n");
			output += TEXT("Basis: ") + target->getEScomponent()->getBasis()->getName() + TEXT("\n");
			output += TEXT("Constraints:\n");
			for (const auto elem : *target->getEScomponent()->getConstraints())
			{
				output += FString(ConstrType.Find(elem->getType())->c_str()) + TEXT("\n");
			}
			return true;
		}
	}
	return false;
}

ExSimComponent* AExSimStorage::getExSmComponent(AActor* actor)
{
	if (actor)
	{
		AExSmplBox * trg = static_cast<AExSmplBox*>(actor);
		return trg->getEScomponent();
	}
	return nullptr;
}

void AExSimStorage::pickActor(AActor* actor, FVector location)
{
	if (actor&&CurrentScene)
	{
		AExSmplBox * target = static_cast<AExSmplBox*>(actor);
		CurrentScene->pickTrgBody(target->getEScomponent(), location);
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

void AExSimStorage::saveExSimComplex(ExSimComplex* target)
{
	AExSimFileManager::es_complex_params * cmplx = new AExSimFileManager::es_complex_params();
	cmplx->string_list.Add("Name",target->getName());
	cmplx->string_list.Add("BasisName",target->getBasis()->getName());
	for (const auto component : *target->getComponents())
	{
		AExSimFileManager::es_component_params * cmpnt = new AExSimFileManager::es_component_params();
		cmpnt->string_list.Add("Name",component->getName());
		cmpnt->string_list.Add("Path",component->getPath());
		for (const auto constr : *component->getConstraints())
		{
			AExSimFileManager::es_constraint_params * p = new AExSimFileManager::es_constraint_params();
			p->string_list.Add("Name", constr->getName());
			p->string_list.Add("Type",FString(ConstrType[constr->getType()].c_str()));
			p->string_list.Add("Parent", (constr->getParent()) ? constr->getParent()->getName() : "NULL");
			p->string_list.Add("NameParent",constr->getParams()->name_p);
			p->string_list.Add("NameTarget",constr->getParams()->name_t);
			
			p->vector_list.Add("AxisParent",constr->getParams()->axis_p);
			p->vector_list.Add("AxisTarget",constr->getParams()->axis_t);
			p->vector_list.Add("DumpAngular",constr->getParams()->dump_ang);
			p->vector_list.Add("DumpLinear",constr->getParams()->dump_lin);
			p->vector_list.Add("LowerLimAngular",constr->getParams()->low_lim_ang);
			p->vector_list.Add("UpperLimAngular",constr->getParams()->upp_lim_ang);
			p->vector_list.Add("PivotParent",constr->getParams()->pivot_p);
			p->vector_list.Add("PivotTarget",constr->getParams()->pivot_t);
			p->vector_list.Add("StiffnessAngular",constr->getParams()->stiff_ang);
			p->vector_list.Add("StiffnessLinear",constr->getParams()->stiff_lin);
			p->vector_list.Add("LowerLimLinear",constr->getParams()->low_lim_lin);
			p->vector_list.Add("UpperLimLinear",constr->getParams()->upp_lim_lin);
			cmpnt->constraints.Add(p);
		}
		cmplx->components.Add(cmpnt);
	}
	ExWorld->ExFileManager->saveEsComplexParams(cmplx);

}

void AExSimStorage::convertExSimComplex(ExSimComplex* target, const AExSimFileManager::es_complex_params* src)
{
	target->setName( src->string_list.FindRef("Name"));
	const FString basis_name = src->string_list.FindRef("BasicName");
	for (const auto & component : src->components)
	{
		ExSimComponent * c = new ExSimComponent();
		c->setName( component->string_list.FindRef("Name"));
		if (c->getName() == basis_name)
			target->setBasis( c);
		c->setPath( component->string_list.FindRef("Path"));
		c->setBasis( target);
		for (const auto & constraint : component->constraints)
		{
			ExSimConstraintPair * p = new ExSimConstraintPair();
			p->setName( constraint->string_list.FindRef("Name"));
			p->setType(  static_cast<ExSimPhyzHelpers::Constraint>(*ConstrType.FindKey(TCHAR_TO_UTF8(*constraint->string_list.FindRef("Type")))));
			p->getParams()->name_p = constraint->string_list.FindRef("NameParent");
			
			p->getParams()->axis_p = constraint->vector_list.FindRef("AxisParent");
			p->getParams()->axis_t = constraint->vector_list.FindRef("AxisTarget");
			p->getParams()->dump_ang = constraint->vector_list.FindRef("DumpAngular");
			p->getParams()->dump_lin = constraint->vector_list.FindRef("DumpLinear");
			p->getParams()->low_lim_ang = constraint->vector_list.FindRef("LowerLimAngular");
			p->getParams()->upp_lim_ang = constraint->vector_list.FindRef("UpperLimAngular");
			p->getParams()->pivot_p = constraint->vector_list.FindRef("PivotParent");
			p->getParams()->pivot_t = constraint->vector_list.FindRef("PivotTarget");
			p->getParams()->stiff_lin = constraint->vector_list.FindRef("StiffnessLinear");
			p->getParams()->stiff_ang = constraint->vector_list.FindRef("StiffnessAngular");
			p->getParams()->low_lim_lin = constraint->vector_list.FindRef("LowerLimLinear");
			p->getParams()->upp_lim_lin = constraint->vector_list.FindRef("UpperLimLinear");
			c->getConstraints()->Add(p);
			
		}
		target->getComponents()->Add(c);
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

void AExSimStorage::resetOptVPP()
{
	const FString vector_str = ExConvert::getStrFromVec(FVector::ZeroVector);
	const FString value_str = ExConvert::getStrFromFloat(0.f);
	const FString name_str("default");

	OptionValuePairsPtr.Empty();
	
	OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::parent_pivot], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::target_pivot], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::low_lim_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::upp_lim_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::low_lim_ang], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::upp_lim_ang], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::stiff_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::stiff_ang], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::dump_lin], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::dump_ang], vector_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::en_spring], "0; 0; 0; 0; 0; 0");
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::parent_name], name_str);
    OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::target_name], name_str);
	OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::constraint_t], ExSimPhyzHelpers::getNameFromConstraint(ExSimPhyzHelpers::Constraint::NONE));
	OptionValuePairsPtr.Add(OptionNamesPtr[EConstraintParamNames::constraint_name],name_str);
}

void AExSimStorage::setOptVPP(ExSimConstraintPair* params)
{
	CurrentConstraintPtr = params;//???
	ConstraintCommander.setActiveConstraint(params->getParams());
	// OptionValuePairsPtr.Empty();

	ExConstraintDict::updateNames(&OptionNamesPtr, params->getType());
	ExConstraintDict::updateValues(&OptionValuesPtr,params->getParams());
	ExConstraintDict::getNameValuePairs(&OptionNamesPtr,&OptionValuesPtr, &OptionValuePairsPtr);

	
}

void AExSimStorage::updateConstraint()
{
	if (CurrentScene&&CurrentConstraintPtr)
	{
		CurrentScene->updateConstraint(CurrentConstraintPtr);
	}
}

void AExSimStorage::updateConstraintCommand(EConstraintParamNames type, FString str)
{
	ConstraintCommander.updateConstraint(type, str);
	//update constraint
	updateConstraint();
	if (EssEvOnConstraintChanged.IsBound())
	{
		EssEvOnConstraintChanged.Broadcast(static_cast<int>(type), str);
	}
}

void AExSimStorage::undoConstraintCommand()
{
	ConstraintCommander.undo();
	//update constraint
	updateConstraint();
	if (EssEvOnConstraintChanged.IsBound())
	{
		EssEvOnConstraintChanged.Broadcast(-1, TEXT("None"));
	}
}


void AExSimStorage::createComplex(ExSimComponent* component, FString new_complex_name)
{
	ExSimComplex * base = new ExSimComplex();
	base->setName( new_complex_name);
	base->setBasis( component);
	base->getComponents()->Add(component);
	ExSimComplex * old = component->getBasis();
	ExSimComplexList.Add(base);
	component->setBasis( base);
	old->getComponents()->Remove(component);
	if (old != ExSimComplexList[0])
	{
		if (old->getComponents()->Num() == 0)
		{
			ExSimComplexList.Remove(old);
		}
	}
	
}




