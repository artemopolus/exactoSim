// Fill out your copyright notice in the Description page of Project Settings.


#include "ExScene.h"
#include <string>
//#include "ExGenerator.h"

#include "exactoSim/Common/ExSimStorage.h"
#include "Generators/CarGenerator.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

// Sets default values
AExScene::AExScene()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void AExScene::addSmplTestObject(FString name, float mass , FVector location, FRotator rotation)
{
	//Blueprint'/Game/Blueprint/Scene/BP_ExSmplBox_Simple.BP_ExSmplBox_Simple'
	const FString path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Simple.BP_ExSmplBox_Simple_C'";
	btRigidBody * result_body;
	addObjByPath(path, name, &result_body, mass, location, rotation);
}

void AExScene::addGenerator(FVector location, FRotator rotation)
{
	if (CurrentGenerator == nullptr)
	{
		const std::string path = "Class'/Game/Blueprint/Scene/BP_ExGenerator.BP_ExGenerator_C'";
		FString fpath(path.c_str());
		UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
		if (obj != nullptr)
		{
			FActorSpawnParameters params;
			params.Name = "TestGenerator";
			AExGenerator *spawned_obj = static_cast<AExGenerator*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
			spawned_obj->Tags.Add(ToCStr(GeneratorTag));
			actor_body_storage elem;
			elem.actor = spawned_obj;
			elem.body = nullptr;
			if (ExPhyzX)
			{
				spawned_obj->ParentScene = this;
				//spawned_obj->generateObj();
			}
			SceneObjects.Add(elem);
			CurrentGenerator = spawned_obj;
    
		}
	}
}



void AExScene::sendCmdToSelected(int type, float value)
{
	FVector loc;
	FRotator rot;
	AExGenerator * target = nullptr;
	if (SceneObjects.Num() > 0)
		target = static_cast<AExGenerator*>(SceneObjects[1].actor);

	int value_int = value;
	
	switch (type)
	{
	case 0:
		// deleteSceneObjByPrefix(target->getGeneratedObjPrefix());
		//target->generateObj();
		removeConstrain();
		break;
	case 1:
		if (target != nullptr)
		loc = target->GetActorLocation();
		loc.Y += value;
		if (target != nullptr)
		target->SetActorLocation(loc);
		break;
	case 2:
		if (target != nullptr)
		rot = target->GetActorRotation();
		rot.Roll += value;
		if (target != nullptr)
		target->SetActorRotation(rot);
		break;
	case 3:
		if (target != nullptr)
		target->generateObj(FVector(0,0,value));
		removeCar();
		break;
	case AExSimStorage::exsim_cmd_type::EXCT_SWITCH:
		UE_LOG(LogTemp, Warning, TEXT("Switch to object number: %d "), value_int  );
		break;
	default:
		break;
	}

}

void AExScene::moveGenerator(FVector loc, FRotator rot)
{
	FString output = "Generator: ";
	if (CurrentGenerator)
	{
		output += "move";
		CurrentGenerator->SetActorLocation(CurrentGenerator->GetActorLocation() + loc);
		CurrentGenerator->SetActorRotation(CurrentGenerator->GetActorRotation() + rot);
	}
	else
	{
		output += "create";
		addGenerator(SpawnGeneratorLoc, FRotator(0,0,0));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, output);
}

void AExScene::sendExtendedCmdToSelected(actor_cmd cmd)
{
	if (CurrentGenerator != nullptr)
	{
		AExGenerator * target = CurrentGenerator;
		switch (cmd.value_int)
		{
		case AExSimStorage::exsim_cmd_type::EXCT_SWITCH:
			UE_LOG(LogTemp, Warning, TEXT("Switch to object number: %s "), *FString(cmd.value_str.c_str() ) );
			target->setGenObjInfo(cmd.value_str);
			break;
		default:
			break;		
		}
	}
}

// Called when the game starts or when spawned
void AExScene::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screen Message"));
	//create object
	//addSmplTestObject(SpawnObjectLoc, rotation);
	//addGenerator(SpawnGeneratorLoc, rotation);
	//addCarGen(SpawnGeneratorLoc, rotation);
	//generateCar();
	//addSmplTestObject("Static",1.0f,FVector(0,0,100));
}

// Called every frame
void AExScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExScene::addObjByPath(FVector location, FRotator rotation, std::string path, std::string name)
{
	addObjByPath(location, rotation, path, name, FVector(0,0,0));	
}

void AExScene::addObjByPath(FVector location, FRotator rotation, std::string path, std::string name, FVector impulse)
{
	FString fpath(path.c_str());
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
	if (obj != nullptr)
	{
		FActorSpawnParameters params;
		params.Name = name.c_str();
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		actor_body_storage elem;
		elem.actor = spawned_obj;
		elem.body = nullptr;
		if (ExPhyzX)
		{
			elem.body = ExPhyzX->AddRigidBody(spawned_obj);
			elem.body->applyCentralImpulse( BulletHelpers::ToBtSize(rotation.RotateVector(impulse)));
		}
		SceneObjects.Add(elem);
	}	
}

bool AExScene::addObjByPath(const FString path, const FString name, btRigidBody** body, float mass, FVector location,
	FRotator rotation, bool use_genloc, FVector impulse, FVector impulse_pos)
{
	if (use_genloc && CurrentGenerator)
	{
		location = CurrentGenerator->GetActorLocation();
		rotation = CurrentGenerator->GetActorRotation();
	}
	bool result = false;
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
	if ((obj != nullptr)&&ExPhyzX)
	{
		FActorSpawnParameters params;
		params.Name = FName(name);
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		spawned_obj->Tags.Add(ToCStr(BaseTag));
		spawned_obj->Tags.Add(ToCStr(PhysicsTag));
		spawned_obj->Tags.Add(ToCStr(DynamicTag));
		*body = ExPhyzX->AddRigidBody(spawned_obj, mass);
		result = true;
	}
	return result;
}

bool AExScene::addObjByPath(ExSimComponent** component, const FString path, const FString name, float mass,
	FVector location, FRotator rotation, bool use_genloc, FVector impulse, FVector impulse_pos)
{
	if (use_genloc && CurrentGenerator)
	{
		location = CurrentGenerator->GetActorLocation();
		rotation = CurrentGenerator->GetActorRotation();
	}
	bool result = false;
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
	if ((obj != nullptr)&&ExPhyzX)
	{
		FActorSpawnParameters params;
		params.Name = FName(name);
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		spawned_obj->Tags.Add(ToCStr(BaseTag));
		spawned_obj->Tags.Add(ToCStr(PhysicsTag));
		spawned_obj->Tags.Add(ToCStr(DynamicTag));
		btRigidBody * body = ExPhyzX->AddRigidBody(spawned_obj, mass);
		if (body)
		{
 			AExSmplBox * target = static_cast<AExSmplBox*>(body->getUserPointer());
			*component = new ExSimComponent();
			target->setEScomponent(*component);
			(*component)->setBody(body);
			(*component)->setTarget( target );
			(*component)->setName( name );
			(*component)->setPath( path );
			result = true;
		}
	}
	return result;
}


void AExScene::addCarGen(FVector location, FRotator rotation)
{

}
void AExScene::deleteSceneObjByPrefix(std::string prefix)
{
	for (int32 i = 0; i < SceneObjects.Num(); i++)
	{
		auto elem = SceneObjects[i];
		std::string name = TCHAR_TO_UTF8(*elem.actor->GetName());
		if (name.rfind(prefix,0) == 0)
		{
			ExPhyzX->removeRigidBody(elem.body);
			elem.actor->Destroy();
			SceneObjects.RemoveAt(i);
			break;
		}		
	}
}

void AExScene::generateCar()
{

	FString path_to_config = FPaths::ProjectContentDir() + TEXT("Options/gen_object_config.json");

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, path_to_config);

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path_to_config))
	{
		return;
	}

	FString json;
	FFileHelper::LoadFileToString(json, *path_to_config);
	/*TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	TSharedPtr<FJsonObject> player = JsonObject->GetObjectField("player");*/
	TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> json_object = MakeShareable(new FJsonObject);
	FJsonSerializer::Deserialize(json_reader, json_object);
	TSharedPtr<FJsonObject> obj_list = json_object->GetObjectField("Objects");
	TSharedPtr<FJsonObject> default_path_to_bp = json_object->GetObjectField("DefaultPathToBP");
	FString dflt_path_prefix = default_path_to_bp->GetStringField("Prefix");	
	FString dflt_path_folder = default_path_to_bp->GetStringField("Folder");
	FString dflt_path_suffix = default_path_to_bp->GetStringField("Suffix");
	FString dflt_path_basis = default_path_to_bp->GetStringField("Basis");
	FString dflt_path_name = default_path_to_bp->GetStringField("Name");


	//Инициализация
	std::string name_trg = "Simple";
	int complex_obj_num;
	complex_obj_num = obj_list->Values.Num();
	auto complex_list = obj_list->Values;
	UE_LOG(LogTemp, Warning, TEXT("Complex objects count : %d "), complex_obj_num  );
	//перечисляем объекты
	for (auto object_in_complex : complex_list)
	{
		FString obj_name = object_in_complex.Key;
		TSharedPtr<FJsonObject> one_obj = obj_list->GetObjectField(obj_name);
		auto elem_list = one_obj->Values;
		int complex_elem_num = elem_list.Num();
		UE_LOG(LogTemp, Warning, TEXT("elements : %s == %d "), *obj_name, complex_elem_num  );
		//перечисляем элементы
		TArray<AExactoPhysics::ConnectedBodies> system;
		for(auto elem_in_object : elem_list)
		{
			FString elem_name = elem_in_object.Key;
			TSharedPtr<FJsonObject> one_elem = one_obj->GetObjectField(elem_name);
			auto param_list = one_elem->Values;
			int param_elem_num = param_list.Num();
			UE_LOG(LogTemp, Warning, TEXT("params : %s == %d "), *elem_name, param_elem_num  );
			//перечисляем параметры
			TSharedPtr<FJsonObject> path_params = one_elem->GetObjectField("PathToBP");
			FString trg_folder = path_params->GetStringField("Folder");
			FString trg_basis = path_params->GetStringField("Basis");
			FString trg_name = path_params->GetStringField("Name");

			FString res_name = trg_basis + trg_name;
			FString res_path = dflt_path_prefix + trg_folder + res_name + TEXT(".") + res_name + dflt_path_suffix;
			UE_LOG(LogTemp, Warning, TEXT("path : %s  "), *res_path  );

			UClass * class_obj = StaticLoadClass(UObject::StaticClass(), nullptr, *res_path);
			
			if (class_obj != nullptr)
			{
				AExactoPhysics::ConnectedBodies elem;
				FActorSpawnParameters spawn_params;
				FVector location;
        			
				TArray<TSharedPtr<FJsonValue>> location_arr = one_elem->GetArrayField("Location");
				location.X = location_arr[0]->AsNumber();
				location.Y = location_arr[1]->AsNumber();
				location.Z = location_arr[2]->AsNumber();
				UE_LOG(LogTemp, Warning, TEXT("location : %s  "), *location.ToString()  );
        
				TSharedPtr<FJsonObject> parent_params = one_elem->GetObjectField("Parent");
				if (parent_params->GetBoolField("enabled"))
				{
					TArray<TSharedPtr<FJsonValue>> arr;
					elem.name_p = parent_params->GetStringField("Name");
					arr = parent_params->GetArrayField("AxisTarget");
					elem.axis_t = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					arr = parent_params->GetArrayField("AxisParent");
					elem.axis_p = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					arr = parent_params->GetArrayField("PivotTarget");
					elem.pivot_t = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					arr = parent_params->GetArrayField("PivotParent");
					elem.pivot_p = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					FString constr_type = parent_params->GetStringField("Constr");
					if (constr_type == "Hinge")
						elem.constr_type = BulletHelpers::HINGE;
					else if (constr_type == "Hinge2")
						elem.constr_type = BulletHelpers::HINGE2;
					else if (constr_type == "G6DOF_Spring")
						elem.constr_type = BulletHelpers::GEN6DOF_SPRING;
					else
						elem.constr_type = BulletHelpers::HINGE;
					arr = parent_params->GetArrayField("EnSpring");
					for (int i = 0; i < 6; i++)
						elem.en_spring[i] = arr[i]->AsNumber();
					arr = parent_params->GetArrayField("UppLimLin");
					elem.upp_lim_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("LowLimLin");
					elem.low_lim_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("UppLimAng");
					elem.upp_lim_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("LowLimAng");
					elem.low_lim_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("StiffLin");
					elem.stiff_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("StiffAng");
					elem.stiff_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("DumpLin");
					elem.dump_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("DumpAng");
					elem.dump_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					
				}
				else
				{
					elem.name_p = "";
				}
				TArray<TSharedPtr<FJsonValue>> rotation_arr = one_elem->GetArrayField("Rotation");
				FRotator rotation(rotation_arr[0]->AsNumber(), rotation_arr[1]->AsNumber(), rotation_arr[2]->AsNumber());	
				UE_LOG(LogTemp, Warning, TEXT("rotation : %s  "), *rotation.ToString()  );
				elem.name_t = one_elem->GetStringField("Name");
				spawn_params.Name = FName(obj_name + TEXT("_") + elem_name + TEXT("_") + FString::FromInt(SystemsList.Num()));
				
				APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(class_obj,&location, &rotation, spawn_params));
				spawned_obj->Tags.Add(ToCStr(BaseTag));
				spawned_obj->Tags.Add(ToCStr(PhysicsTag));
				spawned_obj->Tags.Add(ToCStr(DynamicTag));
				
				elem.target = spawned_obj;
				elem.parent = nullptr;
				elem.trg_body = nullptr;
				elem.trg_constr = nullptr;
				system.Add(elem);
			}
		}
		if ( system.Num() > 0 )
		{
			if (system.Num() > 1)
			{
				for (auto elem : system)
				{
					FString parent_name = elem.name_t;
					AActor * parent_actor = elem.target;
					for (auto & another_elem : system)
					{
						if (parent_name == another_elem.name_p)
						{
							another_elem.parent = parent_actor;
						}
					}
				}
			}
			ExPhyzX->AddComplexBody(&system);
			SystemsList.Add(system);
		}
	}

}

void AExScene::removeCar()
{
	for (int i = 0; i < SystemsList.Num(); i++)
	{
		auto & system = SystemsList[i];
		UE_LOG(LogTemp, Warning, TEXT("System components :  %d "),  system.Num()  );
		for(int j = 0; j < system.Num(); j++)
		{
			auto & component = system[j];
			ExPhyzX->removeRigidBody(component.trg_body);
			component.target->Destroy();
			component.parent = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Remove : %s == %d "), *component.name_t, j  );
		}
		system.Empty();
	}
	SystemsList.Empty();
}

void AExScene::createConstraint(btRigidBody* target, btRigidBody* parent, FExConstraintParams params)
{
	if (ExPhyzX)
	{
		ExPhyzX->createConstraint(target, parent, params);
	}
}

void AExScene::removeConstrain()
{
	for (auto & system : SystemsList)
	{
		for (auto & component : system)
		{
			if (component.trg_constr != nullptr)
			{
				ExPhyzX->removeConstrain(component.trg_constr);
				component.trg_constr = nullptr;
			}
		}
	}
}

void AExScene::updateConstraint(btPoint2PointConstraint* c, FExConstraintParams* params)
{
	if(c)
	{
		c->setPivotA(BulletHelpers::ToBtSize(params->pivot_p));
		c->setPivotB(BulletHelpers::ToBtSize(params->pivot_t));
		c->m_setting.m_tau = params->tau;
		c->m_setting.m_impulseClamp = params->impulse_clamp;
	}
}

void AExScene::updateConstraint(ExSimConstraintPair* pair)
{
	pair->setName(pair->getParams()->name_constraint);
	if (pair->getParams()->constr_type == ExSimPhyzHelpers::Constraint::P2P)
	{
		btPoint2PointConstraint* p = static_cast<btPoint2PointConstraint*>(pair->getConstraint());
		updateConstraint(p, pair->getParams());
	}
	if (pair->getParams()->constr_type == ExSimPhyzHelpers::Constraint::HINGE)
	{
		
	}
	if (pair->getParams()->constr_type == ExSimPhyzHelpers::Constraint::HINGE2)
	{
		
	}
	if (pair->getParams()->constr_type == ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING)
	{
		
	}
}


btTypedConstraint* AExScene::fixP2P(btRigidBody* body, FVector location)
{
	if (body)
	{
		body->setActivationState(DISABLE_DEACTIVATION);
		const btVector3 pivot = BulletHelpers::ToBtSize(location);
		btPoint2PointConstraint * p2p = new btPoint2PointConstraint(*body, pivot);
		ExPhyzX->BtWorld->addConstraint(p2p);
		p2p->m_setting.m_impulseClamp = 30.f;
		p2p->m_setting.m_tau = 0.001f;
		return p2p;
	}
	return nullptr;
}

btTypedConstraint* AExScene::fixP2P(btRigidBody* body, FExConstraintParams* params)
{
	if (body)
	{
		body->setActivationState(DISABLE_DEACTIVATION);
    	const btVector3 pivot = BulletHelpers::ToBtSize(params->pivot_p);
    	btPoint2PointConstraint * p2p = new btPoint2PointConstraint(*body, pivot);
    	ExPhyzX->BtWorld->addConstraint(p2p);
    	p2p->m_setting.m_impulseClamp = params->dump_lin.X;
    	p2p->m_setting.m_tau = params->dump_lin.Y;
		p2p->setPivotB(BulletHelpers::ToBtSize(params->pivot_t));
		return p2p;
	}
	return nullptr;
}

ExSimConstraintPair* AExScene::fixP2P(ExSimComponent* component, FExConstraintParams* params)
{
	if (!component && !params && params->constr_type == ExSimPhyzHelpers::NONE && !component->getBody())
		return nullptr;
	btRigidBody* body = component->getBody();
	body->setActivationState(DISABLE_DEACTIVATION);
	const btVector3 pivot = BulletHelpers::ToBtSize(params->pivot_p);
	btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, pivot);
	ExPhyzX->BtWorld->addConstraint(p2p);
	p2p->m_setting.m_impulseClamp = params->impulse_clamp;
	p2p->m_setting.m_tau = params->tau;
	ExSimConstraintPair* p = new ExSimConstraintPair(component, params);
	params->pivot_p = BulletHelpers::ToUEPos(p2p->getPivotInA());
	params->pivot_t = BulletHelpers::ToUEPos(p2p->getPivotInB());
	p->setConstraint(p2p);
	return p;
}

ExSimConstraintPair* AExScene::fixGen6dofSpring(ExSimComponent* comp_a, ExSimComponent* comp_b, FExConstraintParams* params)
{
	btRigidBody * p_body_a = comp_a->getBody();
	btRigidBody * p_body_b = comp_b->getBody();
	btTransform tr;
	tr.setIdentity();
	p_body_a->setActivationState(DISABLE_DEACTIVATION);
	p_body_b->setActivationState(DISABLE_DEACTIVATION);
	btTransform frameInA, frameInB;
	frameInA = btTransform::getIdentity();
	frameInA.setOrigin(BulletHelpers::ToBtSize(params->pivot_p));
	frameInB = btTransform::getIdentity();
	frameInB.setOrigin(BulletHelpers::ToBtSize(params->pivot_t));
	btGeneric6DofSpringConstraint * g6dof = new btGeneric6DofSpringConstraint(*p_body_a, *p_body_b, frameInA, frameInB, true);
	
	g6dof->setLinearLowerLimit(BulletHelpers::ToBtSize(params->low_lim_lin));
	g6dof->setLinearUpperLimit(BulletHelpers::ToBtSize(params->upp_lim_lin));

	g6dof->setAngularLowerLimit(BulletHelpers::ToBtSize(params->low_lim_ang));
	g6dof->setAngularUpperLimit(BulletHelpers::ToBtSize(params->upp_lim_ang));

	TArray<bool> enables_spring;
	ExConvert::getBoolArrayFromInt(params->enables_spring, &enables_spring, 6);
	for (int i =0; i < enables_spring.Num(); i++)
		g6dof->enableSpring(i, enables_spring[i]);
	
	g6dof->setStiffness(0,params->stiff_lin.X);
	g6dof->setStiffness(1,params->stiff_lin.Y);
	g6dof->setStiffness(2,params->stiff_lin.Z);
	g6dof->setDamping(0, params->dump_lin.X);
	g6dof->setDamping(1, params->dump_lin.Y);
	g6dof->setDamping(2, params->dump_lin.Z);

	ExPhyzX->BtWorld->addConstraint(g6dof);
	params->name_p = comp_a->getName();
	params->name_t = comp_b->getName();
	ExSimConstraintPair* p = new ExSimConstraintPair(comp_a, params);
	p->setConstraint(g6dof);
	return p;
}

ExSimConstraintPair* AExScene::fixHinge2Constraint(ExSimComponent* par, ExSimComponent* trg,
	FExConstraintParams* params)
{
	btRigidBody * p_body_a = par->getBody();
	btRigidBody * p_body_b = trg->getBody();
	p_body_a->setActivationState(DISABLE_DEACTIVATION);
	p_body_b->setActivationState(DISABLE_DEACTIVATION);
	btVector3 p_axis(BulletHelpers::ToBtSize(params->axis_p));
	btVector3 t_axis(BulletHelpers::ToBtSize(params->axis_t));
	btVector3 anchor(BulletHelpers::ToBtSize(params->pivot_p));
	btHinge2Constraint * hinge = new btHinge2Constraint(*p_body_a,*p_body_b, anchor,p_axis, t_axis);
	hinge->setLowerLimit(BulletHelpers::ToBtSize(params->lower_limit));
	hinge->setUpperLimit(BulletHelpers::ToBtSize(params->upper_limit));

	ExPhyzX->BtWorld->addConstraint(hinge);
	params->name_p = par->getName();
	params->name_t = trg->getName();
	ExSimConstraintPair* p = new ExSimConstraintPair(par, params);
	p->setConstraint(hinge);
	return p;
}

ExSimConstraintPair* AExScene::fixHinge1Constraint(ExSimComponent* par, ExSimComponent* trg,
	FExConstraintParams* params)
{
	btRigidBody * p_body_p = par->getBody();
	btRigidBody * p_body_t = trg->getBody();
	p_body_p->setActivationState(DISABLE_DEACTIVATION);
	p_body_t->setActivationState(DISABLE_DEACTIVATION);
	const btVector3 axis_p(BulletHelpers::ToBtSize(params->axis_p));
	const btVector3 axis_t(BulletHelpers::ToBtSize(params->axis_t));
	const btVector3 pivot_p(BulletHelpers::ToBtSize(params->pivot_p));
	const btVector3 pivot_t(BulletHelpers::ToBtSize(params->pivot_t));
	btHingeConstraint * hinge = new btHingeConstraint(*p_body_p,*p_body_t, pivot_p,pivot_t, axis_p, axis_t);
	hinge->setLimit(params->lower_limit, params->upper_limit);
	params->name_p = par->getName();
	params->name_t = trg->getName();
	ExPhyzX->BtWorld->addConstraint(hinge);
	ExSimConstraintPair* p = new ExSimConstraintPair(par, params);
	p->setConstraint(hinge);
	return p;
}

ExSimConstraintPair* AExScene::fixUniConstraint(ExSimComponent* par, ExSimComponent* trg, FExConstraintParams* params)
{
	btRigidBody * p_body_p = par->getBody();
	btRigidBody * p_body_t = trg->getBody();
	p_body_p->setActivationState(DISABLE_DEACTIVATION);
	p_body_t->setActivationState(DISABLE_DEACTIVATION);
	const btVector3 axis_p(BulletHelpers::ToBtSize(params->axis_p));
	const btVector3 axis_t(BulletHelpers::ToBtSize(params->axis_t));
	const btVector3 anchor(BulletHelpers::ToBtSize(params->pivot_p));
	btUniversalConstraint * uni = new btUniversalConstraint(*p_body_p, *p_body_t,anchor, axis_p, axis_t);
	uni->setLowerLimit(BulletHelpers::ToBtSize(params->low_lim_lin.X), BulletHelpers::ToBtSize(params->low_lim_lin.Y));
	uni->setUpperLimit(BulletHelpers::ToBtSize(params->upp_lim_lin.X), BulletHelpers::ToBtSize(params->upp_lim_lin.Y));
	ExPhyzX->BtWorld->addConstraint(uni);
	ExSimConstraintPair* p = new ExSimConstraintPair(par, params);
	p->setConstraint(uni);
	return p;
}

ExSimConstraintPair* AExScene::fixConeTwist(ExSimComponent* par, ExSimComponent* trg, FExConstraintParams* params)
{
	btRigidBody * p_body_p = par->getBody();
	btRigidBody * p_body_t = trg->getBody();
	p_body_p->setActivationState(DISABLE_DEACTIVATION);

	btTransform frame_in_p, frame_in_t;
	frame_in_p = btTransform::getIdentity();
	frame_in_p.getBasis().setEulerZYX(	BulletHelpers::ToBtSize(params->axis_p.X),
										BulletHelpers::ToBtSize(params->axis_p.Y),
										BulletHelpers::ToBtSize(params->axis_p.Z));
	frame_in_p.setOrigin(BulletHelpers::ToBtSize(params->pivot_p));
	
	frame_in_t = btTransform::getIdentity();
	frame_in_t.getBasis().setEulerZYX(	BulletHelpers::ToBtSize(params->axis_t.X),
										BulletHelpers::ToBtSize(params->axis_t.Y),
										BulletHelpers::ToBtSize(params->axis_t.Z));
	frame_in_t.setOrigin(BulletHelpers::ToBtSize(params->pivot_p));

	btConeTwistConstraint * cone = new btConeTwistConstraint(*p_body_p, *p_body_t,frame_in_p, frame_in_t);
	cone->setLimit(
		BulletHelpers::ToBtSize(params->low_lim_lin.X),
		BulletHelpers::ToBtSize(params->low_lim_lin.Y),
		BulletHelpers::ToBtSize(params->low_lim_lin.Z),
		BulletHelpers::ToBtSize(params->lower_limit)
		);
	ExPhyzX->BtWorld->addConstraint(cone);
	ExSimConstraintPair* p = new ExSimConstraintPair(par, params);
	p->setConstraint(cone);
	return p;	
}

ExSimConstraintPair* AExScene::fixSlider(ExSimComponent* par, ExSimComponent* trg, FExConstraintParams* params)
{
	btTransform frame_in_a, frame_in_b;
	frame_in_a = btTransform::getIdentity();
	frame_in_b = btTransform::getIdentity();
	btRigidBody * p_body_p = par->getBody();
	btRigidBody * p_body_t = trg->getBody();
	p_body_p->setActivationState(DISABLE_DEACTIVATION);
	p_body_t->setActivationState(DISABLE_DEACTIVATION);
	btSliderConstraint * sl = new btSliderConstraint(*p_body_p,*p_body_t,frame_in_a, frame_in_b, true);
	sl->setLowerLinLimit(BulletHelpers::ToBtSize(params->low_lim_lin.X));
	sl->setUpperLinLimit(BulletHelpers::ToBtSize(params->low_lim_lin.Y));
	sl->setLowerAngLimit(BulletHelpers::ToBtSize(params->low_lim_ang.X));
	sl->setUpperAngLimit(BulletHelpers::ToBtSize(params->low_lim_ang.Y));
	ExPhyzX->BtWorld->addConstraint(sl);
	ExSimConstraintPair* p = new ExSimConstraintPair(par, params);
	p->setConstraint(sl);
	return p;
}

ExSimConstraintPair* AExScene::fixGear(ExSimComponent* par, ExSimComponent* trg, FExConstraintParams* params)
{
	btRigidBody * p_body_p = par->getBody();
	btRigidBody * p_body_t = trg->getBody();
	p_body_p->setActivationState(DISABLE_DEACTIVATION);
	p_body_t->setActivationState(DISABLE_DEACTIVATION);
	const btVector3 axis_p(BulletHelpers::ToBtSize(params->axis_p));
	 btVector3 axis_t(BulletHelpers::ToBtSize(params->axis_t));
	btQuaternion orn(BulletHelpers::ToBtSize(params->axis_t), params->tau);
	btMatrix3x3 mat(orn);
	axis_t = mat.getRow(1);

	btGearConstraint * g = new btGearConstraint(*p_body_p, *p_body_t, axis_p, axis_t, params->impulse_clamp);
	ExPhyzX->BtWorld->addConstraint(g);
	ExSimConstraintPair* p = new ExSimConstraintPair(par, params);
	p->setConstraint(g);
	return p;
}

ExSimConstraintPair* AExScene::createConstraint(ExSimComponent* par, ExSimComponent* trg, FExConstraintParams* params)
{
	ExSimConstraintPair * pt = nullptr;
	if (params->constr_type == ExSimPhyzHelpers::Constraint::GEN6DOF_SPRING)
	{
		pt = fixGen6dofSpring(par, trg, params);
	}
	if(pt)
	{
		par->getConstraints()->Add(pt);
	}
	return pt;
}

bool AExScene::checkConstraint(ExSimConstraintPair* trg)
{
	if(trg->getConstraint() != nullptr)
		return true;
	return false;
}

btTypedConstraint* AExScene::fixGen6dofSpring(btRigidBody * p_body_a, btRigidBody * p_body_b, FExConstraintParams params)
{
	btTransform tr;
	tr.setIdentity();
	p_body_a->setActivationState(DISABLE_DEACTIVATION);
	p_body_b->setActivationState(DISABLE_DEACTIVATION);
	btTransform frameInA, frameInB;
	frameInA = btTransform::getIdentity();
	frameInA.setOrigin(BulletHelpers::ToBtSize(params.pivot_p));
	frameInB = btTransform::getIdentity();
	frameInB.setOrigin(BulletHelpers::ToBtSize(params.pivot_t));
	btGeneric6DofSpringConstraint * p = new btGeneric6DofSpringConstraint(*p_body_a, *p_body_b, frameInA, frameInB, true);
	
	p->setLinearLowerLimit(BulletHelpers::ToBtSize(params.low_lim_lin));
	p->setLinearUpperLimit(BulletHelpers::ToBtSize(params.upp_lim_lin));

	p->setAngularLowerLimit(BulletHelpers::ToBtSize(params.low_lim_ang));
	p->setAngularUpperLimit(BulletHelpers::ToBtSize(params.upp_lim_ang));
	
	TArray<bool> enables_spring;
	ExConvert::getBoolArrayFromInt(params.enables_spring, &enables_spring, 6);
	for (int i =0; i < enables_spring.Num(); i++)
		p->enableSpring(i, enables_spring[i]);
	
	p->setStiffness(0,params.stiff_lin.X);
	p->setStiffness(1,params.stiff_lin.Y);
	p->setStiffness(2,params.stiff_lin.Z);
	p->setDamping(0, params.dump_lin.X);
	p->setDamping(1, params.dump_lin.Y);
	p->setDamping(2, params.dump_lin.Z);

	ExPhyzX->BtWorld->addConstraint(p);
	return p;
}

void AExScene::pickTrgBody(btRigidBody* body, FVector location)
{
	if (body)
	{
		PickedBody = body;
		PickState = PickedBody->getActivationState();
		PickedBody->setActivationState(DISABLE_DEACTIVATION);
		btVector3 local_pivot = PickedBody->getCenterOfMassTransform().inverse() * BulletHelpers::ToBtSize(location);
		btPoint2PointConstraint *p2p = new btPoint2PointConstraint(*PickedBody, local_pivot);
		PickConstraint = p2p;
		ExPhyzX->BtWorld->addConstraint(p2p);
		p2p->m_setting.m_impulseClamp = 30.f;
		p2p->m_setting.m_tau = 0.001f;
	}
}

void AExScene::pickTrgBody(ExSimComponent* component, FVector location)
{
	btRigidBody * body = component->getBody();
	pickTrgBody(body, location);
}


void AExScene::moveTrgBody(FVector location)
{
	if (PickedBody && PickConstraint)
	{
		btPoint2PointConstraint *p2p = static_cast<btPoint2PointConstraint*>(PickConstraint);
		p2p->setPivotB(BulletHelpers::ToBtSize(location));
	}
}

void AExScene::letTrgBody()
{
	if (PickConstraint)
	{
		PickedBody->forceActivationState(PickState);
		PickedBody->activate();
		ExPhyzX->removeConstrain(PickConstraint);
		////delete PickConstraint;
		PickConstraint = nullptr;
		PickedBody = nullptr;
	}
}

bool AExScene::getTrgBody(AActor** actor)
{
	if (PickedBody)
	{
		*actor = static_cast<AActor*>(PickedBody->getUserPointer());
		if (*actor)
			return true;
	}
	return false;
}

