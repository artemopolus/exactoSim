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
	
	OptionNamesPtr.Add(EnExConstraintParamNames::parent_pivot, "Parent Pivot");
    OptionNamesPtr.Add(EnExConstraintParamNames::target_pivot, "Target Pivot");
    OptionNamesPtr.Add(EnExConstraintParamNames::low_lim_lin, "Low Lim Lin");
    OptionNamesPtr.Add(EnExConstraintParamNames::upp_lim_lin, "Upp Lim Lin");
    OptionNamesPtr.Add(EnExConstraintParamNames::low_lim_ang, "Low Lim Ang");
    OptionNamesPtr.Add(EnExConstraintParamNames::upp_lim_ang, "Upp Lim Ang");
    OptionNamesPtr.Add(EnExConstraintParamNames::en_spring, "Enable Spring");
    OptionNamesPtr.Add(EnExConstraintParamNames::stiff_lin, "Stiff Lin");
    OptionNamesPtr.Add(EnExConstraintParamNames::stiff_ang, "Stiff Ang");
    OptionNamesPtr.Add(EnExConstraintParamNames::dump_lin, "Dump Lin");
    OptionNamesPtr.Add(EnExConstraintParamNames::dump_ang, "Dump Ang");
    OptionNamesPtr.Add(EnExConstraintParamNames::parent_name, "Parent name");
    OptionNamesPtr.Add(EnExConstraintParamNames::target_name, "Target name");
	OptionNamesPtr.Add(EnExConstraintParamNames::constraint_t, "Constraint type");
	OptionNamesPtr.Add(EnExConstraintParamNames::constraint_name, "Constraint Name");



	createNewConstraint(); //TODO: is it necessary????
 
	CoCoCoProvider = new ExCommander();

	CoCoCoProvider->EcEvOnCommand.AddUObject(this, &AExSimStorage::onCommandRegistered);//(this, &AExSimStorage::onCommandRegistered);
	
}




// Called when the game starts or when spawned
void AExSimStorage::BeginPlay()
{
	Super::BeginPlay();
	

	// createTestObjects2();
	// createTestObjects();

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



void AExSimStorage::setTargetWidget(UUserWidget* widget)
{
	CurrentWidget = widget;
}

void AExSimStorage::createTest(FString name, float mass, FVector loc, FRotator rot)
{
	const FString path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Simple.BP_ExSmplBox_Simple_C'";
	
	createComponent(name, path, mass, loc, rot, false);	
}

void AExSimStorage::onCommandRegistered(FExCommonParams * params)
{
	if (params->getType()==EnExParamTypes::CONSTRAINT)
	{
		ExSimConstraintPair * pair = static_cast<ExSimConstraintPair*>(params->getOwner());
		static_assert(pair != nullptr);
		update(pair);
	}
	else if (params->getType()==EnExParamTypes::COMPONENT)
	{
		ExSimComponent * component = static_cast<ExSimComponent*>(params->getOwner());
		static_assert(component != nullptr);
		update(component);
	}
	else if (params->getType()==EnExParamTypes::COMPLEX)
	{
		ExSimComplex * complex = static_cast<ExSimComplex*>(params->getOwner());
		static_assert(complex != nullptr);
		update(complex);
	}
}


void AExSimStorage::createConstraint(ExSimComponent * parent_component, ExSimComponent * target_component)
{
	if (CurrentScene->checkConstraint(CurrentConstraintPtr))
		return;
	//Add to parent complex or create new one
	if (parent_component->getBasis() == ExSimComplexList[0])
	{
		CoCoCoProvider->updateComponent(parent_component->getParams(),EnExComponentParamNames::C_BASIS_NAME, ExSimComplexList[0]->getName());
	}
	//create constraint 	
	// ExSimConstraintPair * p = CurrentScene->createConstraint(parent_component, target_component, CurrentConstraintPtr->getParams());
	// delete CurrentConstraintPtr;
	// CurrentConstraintPtr = p;
	CoCoCoProvider->createConstraint(CurrentConstraintPtr->getParams());
	
	//rebase component
	if (target_component)
		CoCoCoProvider->updateComponent(target_component->getParams(), EnExComponentParamNames::C_BASIS_NAME, parent_component->getBasis()->getName());
	
}
void AExSimStorage::createConstraint(FString* target, FString* parent)
{
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

void AExSimStorage::selectComplex(ExSimComponent* trg)
{
	CurrentComplex = trg->getBasis();
}

void AExSimStorage::loadComplex()
{
	ExSimComplex * complex = new ExSimComplex();
	ExWorld->ExFileManager->loadEsComplexParams(TEXT("Magnet"), complex);
	if (complex)
		createComplex(complex);
}

void AExSimStorage::saveComplex()
{
	saveComplex(CurrentComplex);
}

void AExSimStorage::saveComplex(ExSimComplex* target)
{
	if (target)
		ExWorld->ExFileManager->save(target);
}

void AExSimStorage::saveComplex(int index)
{
	saveComplex(ExSimComplexList[index]);
}

void AExSimStorage::loadExSimComplex()
{
	TArray<AExSimFileManager::es_complex_params * > c_p_list;
	ExWorld->ExFileManager->loadEsComplexParams(c_p_list);
}

void AExSimStorage::createNewConstraint()
{

	ExSimConstraintPair * params = new ExSimConstraintPair();

	setOptVPP(params);
}

void AExSimStorage::updateOptVPP()
{
	setOptVPP(CurrentConstraintPtr);
}

void AExSimStorage::setOptVPP(ExSimConstraintPair* params)
{
	CurrentConstraintPtr = params;//???
	CoCoCoProvider->setActiveConstraint(params->getParams());

	ExConstraintDict::updateNames(&OptionNamesPtr, params->getParams()->constr_type);
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

void AExSimStorage::update(ExSimConstraintPair* pair)
{
	CurrentScene->updateConstraint(pair);
}

void AExSimStorage::update(ExSimComponent* component)
{
	component->setBasis(&ExSimComplexList);
	CurrentScene->updateComponent(component);
}

void AExSimStorage::update(ExSimComplex* complex)
{
	if(complex->getParams()->isMarkedToCreate())
	{
		ExSimComplexList.Add(complex);
		complex->getParams()->resetMarkers();
		complex->addComponents(ExSimComplexList[0]->getComponentsList());
	}
	else if (complex->getParams()->isMarkedToDelete())
	{
		complex->moveComponents(ExSimComplexList[0]);
		ExSimComplexList.Remove(complex);
		delete complex;
		return;
	}
	complex->update();
}

void AExSimStorage::updateConstraintCommand(EnExConstraintParamNames type, FString str)
{
	CoCoCoProvider->updateConstraint(type, str);
	//update constraint
	updateConstraint();
	if (EssEvOnConstraintChanged.IsBound())
	{
		EssEvOnConstraintChanged.Broadcast(static_cast<int>(type), str);
	}
}

void AExSimStorage::undoConstraintCommand()
{
	CoCoCoProvider->undo();
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
	base->getComponentsList()->Add(component);
	ExSimComplex * old = component->getBasis();
	ExSimComplexList.Add(base);
	component->setBasis( base);
	old->getComponentsList()->Remove(component);
	if (old != ExSimComplexList[0])
	{
		if (old->getComponentsList()->Num() == 0)
		{
			ExSimComplexList.Remove(old);
		}
	}
	
}
void AExSimStorage::createComponent(FString name, FString path, float mass, FVector loc, FRotator rot, bool use_genloc)
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
				complex->getComponentsList()->Add(component);
			}
		}
	}
}



void AExSimStorage::createComplex(ExSimComplex* complex)
{
	for (ExSimComponent* component : *complex->getComponentsList())
	{
		const auto comp_params = component->getParams();
		CurrentScene->addObjByPath(component);
		component->setBasis(complex);
		if (comp_params->Name == complex->getBasisName())
			complex->setBasis(component);
	}
	for (ExSimComponent* component : *complex->getComponentsList())
	{
		for (ExSimConstraintPair * pair : *component->getConstraints())
		{
			const FExConstraintParams * params = pair->getParams();
			ExSimComponent * parent = complex->getComponent(params->name_p);
			ExSimComponent * target = complex->getComponent(params->name_t);
			CurrentScene->createConstraint(pair,parent,target);
		}
	}
	ExSimComplexList.Add(complex);
}

	

void AExSimStorage::createComponent(ExSimComponent* component)
{
	ExSimComplex* complex = ExSimComplexList[0]; //to free object list
	const auto comp_params = component->getParams();
	CurrentScene->addObjByPath(component);
	component->setBasis(complex);
	if (comp_params->Name == complex->getBasisName())
		complex->setBasis(component);
}

void AExSimStorage::createConstraintPair(ExSimConstraintPair* pair)
{
	const FExConstraintParams* params = pair->getParams();
	ExSimComponent *parent = nullptr, *target = nullptr;
	for (int32 i = 0; i < ExSimComplexList.Num() && (parent == nullptr); i++)
	{
		const auto complex = ExSimComplexList[i];
		parent = complex->getComponent(params->name_p);
	}
	for (int32 i = 0; i < ExSimComplexList.Num() && (target == nullptr); i++)
	{
		const auto complex = ExSimComplexList[i];
		target = complex->getComponent(params->name_t);
	}
	CurrentScene->createConstraint(pair, parent, target);
}

void AExSimStorage::createTestObjects()
{
	const FString magnet_name = TEXT("MagnetT");
	const FString spring_name = TEXT("SpringT");
	
	const FVector magnet_pos(0, 100, 200);
	const FVector spring_pos(0, 150, 250);
	
	const FVector magnet_relpivot0(0, 0, 20);
	const FVector magnet_relpivot1(20, 0, 20);
	const FVector spring_pt(0, 0, 20);
	


	float mass = 1.0f;
	float stiff = Stiffness;
	float dump = Dumping;

	ExSimComplex * basic = new ExSimComplex();
	basic->setName("Magnet");
	basic->setBasisName(magnet_name);
	{
		ExSimComponent * component = new ExSimComponent();
		FExComponentParams * params = new FExComponentParams();
		params->Mass = 100.f;
		params->Name = magnet_name;
		params->Path = TEXT("Class'/Game/Blueprint/Scene/BP_ESB_Magnet.BP_ESB_Magnet_C'");
		params->Position = magnet_pos;
		params->Rotation = FRotator::ZeroRotator;
		component->setParams(params);
		{
			FExConstraintParams* pparams = new FExConstraintParams();
			pparams->pivot_p = magnet_relpivot0;
			pparams->name_p = magnet_name;
			pparams->constr_type = ExSimPhyzHelpers::Constraint::P2P;
			pparams->name_constraint = TEXT("Magnet_P2P_1");
			pparams->impulse_clamp = 30.f;
			pparams->tau = 0.001f;
			ExSimConstraintPair * pair = new ExSimConstraintPair();
			pair->setParams(pparams);
			component->addConstraint(pair);
		}
		{
			FExConstraintParams* pparams = new FExConstraintParams();
			pparams->pivot_p = magnet_relpivot1;
			pparams->name_p = magnet_name;
			pparams->constr_type = ExSimPhyzHelpers::Constraint::P2P;
			pparams->name_constraint = TEXT("Magnet_P2P_2");
			pparams->impulse_clamp = 30.f;
			pparams->tau = 0.001f;
			ExSimConstraintPair* pair = new ExSimConstraintPair();
			pair->setParams(pparams);
			component->addConstraint(pair);
		}
		{
		// FExConstraintParams* params = new FExConstraintParams();
  //   	params->pivot_p = PivotP;
  //   	params->pivot_t = PivotT;
  //   	params->low_lim_lin = LowLimLin;
  //   	params->upp_lim_lin = UppLimLin;
  //   	params->low_lim_ang = FVector::ZeroVector;
  //   	params->upp_lim_ang = FVector::ZeroVector;
  //   	params->stiff_lin = FVector(stiff, stiff, stiff);
  //   	params->dump_lin = FVector(dump, dump, dump);
  //   	params->enables_spring = 7;
  //   	params->constr_type = ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING;
  //   	params->name_constraint = TEXT("g6dof spring");
			FExConstraintParams* pparams = new FExConstraintParams();
			pparams->name_p = magnet_name;
			pparams->name_t = spring_name;
			pparams->constr_type = ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING;
			pparams->name_constraint = TEXT("Magnet_6dof");
			// pparams->impulse_clamp = 30.f;
			// pparams->tau = 0.001f;
			pparams->pivot_p = PivotP;
			pparams->pivot_t = PivotT;
			pparams->low_lim_lin = LowLimLin;
			pparams->upp_lim_lin = UppLimLin;
			pparams->low_lim_ang = FVector::ZeroVector;
			pparams->upp_lim_ang = FVector::ZeroVector;
			pparams->stiff_lin = FVector(stiff, stiff, stiff);
			pparams->dump_lin = FVector(dump, dump, dump);
			pparams->enables_spring = 7;
			ExSimConstraintPair* pair = new ExSimConstraintPair();
			pair->setParams(pparams);
			component->addConstraint(pair);
		}
		basic->addComponent(component);
	}

	{
		ExSimComponent* component = new ExSimComponent();
		FExComponentParams* params = new FExComponentParams();
		params->Mass = 1.f;
		params->Name = spring_name;
		params->Path = TEXT("Class'/Game/Blueprint/Scene/BP_ESB_Spring.BP_ESB_Spring_C'");
		params->Position = spring_pos;
		params->Rotation = FRotator::ZeroRotator;
		component->setParams(params);


		{
			FExConstraintParams* pparams = new FExConstraintParams();
			pparams->pivot_p = spring_pt;
			pparams->name_p = spring_name;
			pparams->constr_type = ExSimPhyzHelpers::Constraint::P2P;
			pparams->name_constraint = TEXT("Spring_P2P_1");
			pparams->impulse_clamp = 30.f;
			pparams->tau = 0.001f;
			ExSimConstraintPair* pair = new ExSimConstraintPair();
			pair->setParams(pparams);
			component->addConstraint(pair);
		}
		
		basic->addComponent(component);
	}

	createComplex(basic);
	
}

void AExSimStorage::createTestObjects2()
{
	const FString magnet_name = "Magnet";
	const FString spring_name = "Spring";
	const FVector magnet_pos(0, 0, 200);
	const FVector spring_pos(0, 50, 250);
	const FVector magnet_relpivot0(0, 0, 20);
	const FVector magnet_relpivot1(20, 0, 20);
	const FVector spring_pt(0, 0, 20);
	const float stiff = Stiffness;
	const float dump = Dumping;



	FString path = "Class'/Game/Blueprint/Scene/BP_ESB_Magnet.BP_ESB_Magnet_C'";
	createComponent(magnet_name, path, 100.0f, magnet_pos, FRotator(0, 0, 0), false);
	
	path = "Class'/Game/Blueprint/Scene/BP_ESB_Spring.BP_ESB_Spring_C'";
	createComponent(spring_name, path, 1.0f, spring_pos, FRotator(0, 0, 0), false);
	
	// path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Simple.BP_ExSmplBox_Simple_C'";
	// createComponent("Hello", path, 1.0f, FVector(0, -50, 50), FRotator(0, 0, 0), false);
	// createComponent("Cubic", path, 1.0f, FVector(0, 50, 50), FRotator(0, 0, 0), false);
	
	
	
	ExSimComponent* magnet_target = nullptr;
	
	for (auto& system : ExSimComplexList)
	{
		for (auto& component : *system->getComponentsList())
		{
			if (component->getName() == magnet_name)
			{
				{
					FExConstraintParams* pp = new FExConstraintParams();
					pp->pivot_p = magnet_relpivot0;
					// pp->axis_p = component->getTarget()->GetActorLocation();
					pp->name_p = component->getName();
					pp->constr_type = ExSimPhyzHelpers::Constraint::P2P;
					pp->name_constraint = magnet_name + TEXT("_1");
					pp->impulse_clamp = 30.f;
					pp->tau = 0.001f;
					ExSimConstraintPair* p = CurrentScene->fixP2P(component, pp);
					component->getConstraints()->Add(p);
				}
				
				{
					FExConstraintParams* pp = new FExConstraintParams();
					pp->pivot_p = magnet_relpivot1;
					// pp->axis_p = component->getTarget()->GetActorLocation();
					pp->name_p = component->getName();
					pp->constr_type = ExSimPhyzHelpers::Constraint::P2P;
					pp->name_constraint = magnet_name + TEXT("_2");
					pp->impulse_clamp = 30.f;
					pp->tau = 0.001f;
					ExSimConstraintPair* p = CurrentScene->fixP2P(component, pp);
					component->getConstraints()->Add(p);
				}
				
	
				magnet_target = component;
				break;
			}
		}
	}
	
	
	
	if (magnet_target)
	{
		createComplex(magnet_target, magnet_name);
		ExSimComponent* spring = nullptr;
		for (auto& component : *ExSimComplexList[0]->getComponentsList())
		{
			if (component->getName() == spring_name)
				spring = component;
		}
	
		if (spring)
		{
			FExConstraintParams* fix_params = new FExConstraintParams();
			fix_params->pivot_p = spring_pt;
			// fix_params->axis_p = spring->getTarget()->GetActorLocation();
			fix_params->name_p = spring->getName();
			fix_params->constr_type = ExSimPhyzHelpers::Constraint::P2P;
			fix_params->name_constraint = TEXT("P2P magnet");
			fix_params->impulse_clamp = 30.f;
			fix_params->tau = 0.001f;
			ExSimConstraintPair* p = CurrentScene->fixP2P(spring, fix_params);
			spring->getConstraints()->Add(p);
	
			FExConstraintParams* params = new FExConstraintParams();
			params->pivot_p = PivotP;
			params->pivot_t = PivotT;
			params->low_lim_lin = LowLimLin;
			params->upp_lim_lin = UppLimLin;
			params->low_lim_ang = FVector::ZeroVector;
			params->upp_lim_ang = FVector::ZeroVector;
			params->stiff_lin = FVector(stiff, stiff, stiff);
			params->dump_lin = FVector(dump, dump, dump);
			params->enables_spring = 7;
			params->constr_type = ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING;
			params->name_constraint = TEXT("g6dof spring");
			ExSimConstraintPair* gen = CurrentScene->fixGen6dofSpring(magnet_target, spring, params);
			// spring->getConstraints()->Add(gen);
			magnet_target->getConstraints()->Add(gen);
	
			
			spring->getBasis()->getComponentsList()->Remove(spring);
			spring->setBasis(magnet_target->getBasis());
			
			magnet_target->getBasis()->getComponentsList()->Add(spring);
		}
	}
}
