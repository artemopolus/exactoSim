// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimMainWidget.h"

#include "ExComboWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "exactoSim/Scene/ExSmplBox.h"
#include "ThirdParty/RenderDoc/renderdoc_app.h"

void UExSimMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Initialize();
}

void UExSimMainWidget::initializeCanvas(const int32 pixelsH, const int32 pixelsV)
{
	//dynamic texture initialization
	CanvasWidth = pixelsH;
	CanvasHeight = pixelsV;

	RawDataOutput = UTexture2D::CreateTransient(CanvasWidth, CanvasHeight);
#if WITH_EDITORONLY_DATA
	RawDataOutput->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif
	RawDataOutput->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	RawDataOutput->SRGB = 1;
	RawDataOutput->AddToRoot();
	RawDataOutput->Filter = TextureFilter::TF_Nearest;
	RawDataOutput->UpdateResource();

	EchoUpdateTextureRegion = std::unique_ptr<FUpdateTextureRegion2D>(new FUpdateTextureRegion2D(0, 0, 0, 0, CanvasWidth, CanvasHeight));


	// buffers initialization
	BytesPerPixel = 4; // r g b a
	BufferPitch = CanvasWidth * BytesPerPixel;
	BufferSize = CanvasWidth * CanvasHeight * BytesPerPixel;
	CanvasPixelData = std::unique_ptr<uint8[]>(new uint8[BufferSize]);

	clearCanvas();
}

void UExSimMainWidget::updateCanvas()
{
	if (EchoUpdateTextureRegion)
	{
		RawDataOutput->UpdateTextureRegions((int32)0, (uint32)1, EchoUpdateTextureRegion.get(), (uint32)BufferPitch, (uint32)BytesPerPixel, CanvasPixelData.get());
	}	
}

void UExSimMainWidget::clearCanvas()
{
	uint8* canvas_pixel_ptr = CanvasPixelData.get();
	for (int i = 0; i < CanvasWidth * CanvasHeight; ++i)
	{
		setPixelColor(canvas_pixel_ptr, 255, 255, 255, 0); //white, set alpha to zero
		canvas_pixel_ptr += BytesPerPixel;
	}
}

void UExSimMainWidget::testDrawFunction()
{
	clearCanvas();

	for (int x = 0; x < 50; x++)
	{
		for (int y = 0; y < 50; y++)
			drawPtOnCanvas(x, 50 + y, 0, 0, 0, 100);
		

	}
	updateCanvas();
}




void UExSimMainWidget::setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha)
{
	*pointer = blue; //b
	*(pointer + 1) = green; //g
	*(pointer + 2) = red; //r
	*(pointer + 3) = alpha; //a
}

void UExSimMainWidget::drawPtOnCanvas(int32 x, int32 y, uint8 red, uint8 green, uint8 blue, uint8 alpha)
{
	uint8* canvasPxPtr = CanvasPixelData.get() + (x + y * CanvasWidth) * BytesPerPixel;
	setPixelColor(canvasPxPtr, red, green, blue, alpha);
}

void UExSimMainWidget::updateDebugText(const std::string str)
{
	if (DebugText)
	{
		const std::string output = std::string("Debug info: ") + str;
		DebugText->SetText(FText::FromString(output.c_str()));
	}
}

void UExSimMainWidget::updateSwitchObjText(const std::string str)
{
	if (SwitchObjText)
	{
		SwitchObjText->SetText(FText::FromString(str.c_str()));
	}
}

void UExSimMainWidget::updateSwitchObjText(const FString str)
{
	if (SwitchObjText)
	{
		SwitchObjText->SetText(FText::FromString(str));
	}
}

void UExSimMainWidget::setVisibilityOptionsPanel(bool onoff)
{
	ESlateVisibility value = ESlateVisibility::Visible;
	if(onoff)
	{
		value = ESlateVisibility::Visible;
	}
	else
	{
		value = ESlateVisibility::Hidden;
	}
	
	
}


void UExSimMainWidget::onSwitchObjButtonClicked()
{
	std::string start = "Gen. Object: ";
	std::string * str = nullptr;
	while(str == nullptr)
	{
		str = DataStorage->GenObjType.Find(++GenObjKey);
		if (GenObjKey >= AExSimStorage::exsim_genobj_type::EXGT_END)
			GenObjKey = -1;
	}
	if (str != nullptr)
	{
		start += *str;
		updateSwitchObjText(start);
		if (DataStorage)
			DataStorage->setSceneObjName("Test", FString(str->c_str()));
			//DataStorage->registerExtendedCmd(AExSimStorage::exsim_cmd_type::EXCT_SWITCH, GenObjKey);
		//DataStorage->registerCmdToSelected(AExSimStorage::exsim_cmd_type::EXCT_SWITCH, static_cast<float>(GenObjKey - 1));
	}
}

void UExSimMainWidget::onApplyConstrButtonClicked()
{
	std::string start = "Constrain: ";
	std::string * str = nullptr;
	while(str == nullptr)
	{
		str = DataStorage->ConstrType.Find(++ConstrKey);
		if (ConstrKey >= BulletHelpers::Constr::NONE)
			ConstrKey = -1;
	}	
	if (str != nullptr)
	{
		start += *str;
		if(ApplyConstrText)
			ApplyConstrText->SetText(FText::FromString(start.c_str()));
	}	
}





static void sendDebug(FString text)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, text);
}

void UExSimMainWidget::setupConstrainOptions(FVector2D loc, AActor *actor)
{
	if (DataStorage)
	{
		FString info = "";
		if (DataStorage->touchActor(actor, info))
		{
			if (OptionsPanel)
			{
				setVisibilityOptionsPanel(true);
				UCanvasPanelSlot *pt = static_cast<UCanvasPanelSlot*> (OptionsPanel->Slot);
				loc -= pt->GetSize()/2;
				pt->SetPosition(loc);
		
			}
			CurrentActor = DataStorage->getExSmComponent(actor);
			FString pair_info = "Pair info:";
			pair_info += getParTrgInfo();
			pair_info += info;
			DebugText->SetText(FText::FromString(pair_info));



			FString out = TEXT("Constraint  pairs list:\n");
			for(int i = 0; i < CurrentActor->Constraints.Num(); i++)
			{
				const FString n = CurrentActor->Constraints[i]->name;
				addButtonToTempList(n, i);
				out += FString::FromInt(i) + TEXT("\t ") + n + TEXT("\n");
			}
			sendDebug(out);
		}
	}
}

void UExSimMainWidget::onGenerateButtonClicked()
{
	if (DataStorage)
	{
		//DataStorage->createSceneObj();
		//DataStorage->saveExSimComplex(1);
		DataStorage->loadExSimComplex();
	}
}

void UExSimMainWidget::onChangeModeButtonClicked()
{
	if (ChangeModeText)
	{
		FString output = "On parent button click! ";
		output = DataStorage->switchMode();
		sendDebug(output);
		ChangeModeText->SetText(FText::FromString(output));
	}
}




UExSimMainWidget::~UExSimMainWidget()
{
	
}


void UExSimMainWidget::setSelectorClass(UClass* tmpl)
{
	SelectorClass = tmpl;
}

void UExSimMainWidget::setButtonClass(UClass* tmpl)
{
	ButtonClass = tmpl;
}

void UExSimMainWidget::setOptionClass(UClass* tmpl)
{
	OptionClass = tmpl;
}

void UExSimMainWidget::setComboClass(UClass* tmpl)
{
	ComboClass = tmpl;
}

void UExSimMainWidget::addOptionToStorage(FString name, FString value)
{
	UExEditableWidget * Menu = CreateWidget<UExEditableWidget>(this, OptionClass);
	OptionsList.Add(Menu);
	Menu->ValueName->SetText(FText::FromString(name));
	Menu->ValueText->SetText(FText::FromString(value));
	Menu->initEditable( 0);
	
	Menu->onFullTextChanged.AddDynamic(this, &UExSimMainWidget::onConstraintTypeChanged);
	StorageWrapBox->AddChild(Menu);
}
void UExSimMainWidget::onConstraintTypeChanged(FText text, int type, int pt)
{
}
void UExSimMainWidget::addButtonToStorage(FString name)
{
	//if (!OptionsButton_Ok)
	//{
		OptionsButton_Ok = CreateWidget<UExButtonWidget>(this, ButtonClass);
		StorageWrapBox->AddChild(OptionsButton_Ok);
		OptionsButton_Ok->setName(name);
		OptionsButton_Ok->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onOptionsButtonOkClicked);
	//}
}

void UExSimMainWidget::addSelectToStorage(FString name, TArray<FString> option_list)
{
	UExSelector * selector = CreateWidget<UExSelector>(this, SelectorClass);
	SelectorList.Add(selector);
	selector->setSelectorText(name);
	for (const auto option : option_list)
	{
		selector->addSelectorValue(option);
	}
	StorageWrapBox->AddChild(selector);
}

void UExSimMainWidget::addConstraintButtonOk()
{
	UExButtonWidget * bt = CreateWidget<UExButtonWidget>(this, ButtonClass);
	StorageWrapBox->AddChild(bt);
	bt->setName("ok");
	bt->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onOptionsButtonOkClicked);

	OptionsButton_Ok = bt;
}
void UExSimMainWidget::onTempListButtonClicked()
{
	for(int i = 0; i < ButtonTempList.Num(); i++)
	{
		if (ButtonTempList[i]->IsHovered())
		{
			sendDebug(ButtonTempList[i]->getButtonName());
			
		}
	}
	clearButtonTempList();
}
void UExSimMainWidget::addButtonToTempList(const FString name, const int tag)
{
	UExButtonWidget * bt = CreateWidget<UExButtonWidget>(this, ButtonClass);
	bt->setName(name);
	bt->tag = tag;
	bt->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onTempListButtonClicked);
	TempWrapBox->AddChild(bt);
	ButtonTempList.Add(bt);
}

void UExSimMainWidget::clearButtonTempList()
{
	for(auto button : ButtonTempList)
	{
		button->RemoveFromParent();
	}
	TempWrapBox->ClearChildren();
	ButtonTempList.Empty();
}

void UExSimMainWidget::addConstraintButtonEsc()
{
	UExButtonWidget * bt = CreateWidget<UExButtonWidget>(this, ButtonClass);
	StorageWrapBox->AddChild(bt);
	bt->setName("esc");
	bt->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onConstraintEscClicked);

	OptionsButton_Esc = bt;
}

void UExSimMainWidget::addConstraintButtonReset()
{
	UExButtonWidget * bt = CreateWidget<UExButtonWidget>(this, ButtonClass);
	StorageWrapBox->AddChild(bt);
	bt->setName("reset");
	bt->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onConstraintResetClicked);

	OptionsButton_Reset = bt;
}

AActor* UExSimMainWidget::getParentActor()
{
	return ParentActor->Target;
}

AActor* UExSimMainWidget::getTargetActor()
{
	return TargetActor->Target;
}

bool UExSimMainWidget::isParTrgPair()
{
	if (ParentActor && TargetActor)
		return true;
	return false;
}

FString UExSimMainWidget::getParTrgInfo()
{
	FString info = "";
	if (ParentActor)
		info = TEXT("\npar: ") + ParentActor->getName();
	if (TargetActor)
		info += TEXT("\ntrg: ") + TargetActor->getName();
	return info;
}

bool UExSimMainWidget::getVectorFromString(FString list, FString splitter, FVector & out)
{
	TArray<FString> values;
	list.ParseIntoArray(values, *splitter, true);
	if (values.Num() == 3)
	{
		out.X = FCString::Atof(*values[0]);	
		out.Y = FCString::Atof(*values[1]);	
		out.Z = FCString::Atof(*values[2]);	
		return true;
	}
	return false;
}



bool UExSimMainWidget::checkBoolArrayOption(UExEditableWidget * option, AExactoPhysics::es_options_list checker, TArray<bool> & vect)
{
	if (option->ValueName->GetText().ToString() == OptionNames[ checker ])
	{
		TArray<FString> values;
		FString list = option->ValueText->GetText().ToString();
		list.ParseIntoArray(values, TEXT(";"), true);
		for (auto & val :values)
		{
			if (FCString::Atoi(*val) == 0)
				vect.Add(false);
			else
				vect.Add(true);
		}
		return true;
	}
	return false;
}

bool UExSimMainWidget::checkStringOption(UExEditableWidget* option, AExactoPhysics::es_options_list checker,
	FString& name)
{
	if (option->ValueName->GetText().ToString() == OptionNames.FindRef( checker ))
	{
		name = option->ValueText->GetText().ToString();
		return true;
	}
	return false;	
}

bool UExSimMainWidget::checkVectorOption(UExEditableWidget * option, AExactoPhysics::es_options_list checker, FVector & vect)
{
	if (option->ValueName->GetText().ToString() == OptionNames[ checker ])
	{
		getVectorFromString(option->ValueText->GetText().ToString(), TEXT(";"),vect);
		return true;
	}
	return false;
}

void UExSimMainWidget::onOptionsButtonOkClicked()
{

	AExactoPhysics::es_constraint * params = new AExactoPhysics::es_constraint();

	for (auto & option : OptionsList)
	{
		checkVectorOption(option, AExactoPhysics::es_options_list::parent_pivot, params->pivot_p);	
		checkVectorOption(option, AExactoPhysics::es_options_list::target_pivot, params->pivot_t);
		checkVectorOption(option, AExactoPhysics::es_options_list::parent_axis, params->axis_p);
		checkVectorOption(option, AExactoPhysics::es_options_list::target_axis, params->axis_t);
		
		checkVectorOption(option, AExactoPhysics::es_options_list::low_lim_lin, params->low_lim_lin);	
		checkVectorOption(option, AExactoPhysics::es_options_list::upp_lim_lin, params->upp_lim_lin);	
		checkVectorOption(option, AExactoPhysics::es_options_list::low_lim_ang, params->low_lim_ang);	
		checkVectorOption(option, AExactoPhysics::es_options_list::upp_lim_ang, params->upp_lim_ang);	
		checkVectorOption(option, AExactoPhysics::es_options_list::stiff_lin, params->stiff_lin);	
		checkVectorOption(option, AExactoPhysics::es_options_list::stiff_ang, params->stiff_ang);	
		checkVectorOption(option, AExactoPhysics::es_options_list::dump_lin, params->dump_lin);
		checkVectorOption(option, AExactoPhysics::es_options_list::dump_ang, params->dump_ang);
		checkStringOption(option, AExactoPhysics::es_options_list::parent_name, params->name_p);
		checkStringOption(option, AExactoPhysics::es_options_list::target_name, params->name_t);
	}

	//delete
	deleteConstraintOptions();

	params->constr_type = SelectedConstraintType;

	DataStorage->createConstraint(ParentActor->Target, params);
}


void UExSimMainWidget::deleteConstraintOptions()
{
	OptionsButton_Ok->RemoveFromParent();
	OptionsButton_Esc->RemoveFromParent();
	
	StorageWrapBox->ClearChildren();
	for (auto & option : OptionsList)
		option->RemoveFromParent();
	OptionsList.Empty();
	for (auto & select : SelectorList)
		select->RemoveFromRoot();
	SelectorList.Empty();	
}


void UExSimMainWidget::onConstraintEscClicked()
{
	deleteConstraintOptions();
}

void UExSimMainWidget::onConstraintResetClicked()
{
}

void UExSimMainWidget::onConstrHingeButtonClicked()
{
	addOptionToStorage("Pivot Parent","0.0; 0.0; 0.0;");
}
void UExSimMainWidget::addInputTable()
{
	UExComboWidget * bt = CreateWidget<UExComboWidget>(this, ComboClass);
    StorageWrapBox->AddChild(bt);
	for (int i = 0; i < static_cast<int>(BulletHelpers::NONE); i++)
		bt->ValueComboBox->AddOption(BulletHelpers::getNameOfConstraint(static_cast<BulletHelpers::Constr>(i)));
	
	for (auto option : OptionNames)
	{
		FString name = option.Value;
		FString * value = OptionValuePairs.Find(name);
		if (value)
			addOptionToStorage(name, *value);
	}
	addConstraintButtonOk();
	addConstraintButtonEsc();

	FString out = TEXT("Add table") + BulletHelpers::getNameOfConstraint(SelectedConstraintType);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, out);
}



void UExSimMainWidget::onConstrGen6dofSpringButtonClicked()
{
	if (!OptionNames.Num())
		OptionNames = DataStorage->OptionNamesPtr;
	if (!OptionValuePairs.Num())
		OptionValuePairs = DataStorage->OptionValuePairsPtr;

	SelectedConstraintType = BulletHelpers::Constr::GEN6DOF_SPRING;
	addInputTable();
}

void UExSimMainWidget::onConstrP2PButtonClicked()
{
	if (!ParentActor)
		return;
	if (!OptionNames.Num())
		OptionNames = DataStorage->OptionNamesPtr;
	if (!OptionValuePairs.Num())
		OptionValuePairs = DataStorage->OptionValuePairsPtr;
	SelectedConstraintType = BulletHelpers::Constr::P2P;

	FString name = OptionNames[AExactoPhysics::es_options_list::parent_pivot];
	FString * value = OptionValuePairs.Find(name);
	if (value)
		addOptionToStorage(name, *value);
	name =  OptionNames[AExactoPhysics::es_options_list::parent_name];
	value = OptionValuePairs.Find(name);
	*value += TEXT("_") + ParentActor->getName() + TEXT("_P2P");
	if (value)
		addOptionToStorage(name, *value);

	name =  OptionNames[AExactoPhysics::es_options_list::target_pivot];
	value = OptionValuePairs.Find(name);
	if (value)
		addOptionToStorage(name, *value);
	
	name =  OptionNames[AExactoPhysics::es_options_list::dump_lin];
	value = OptionValuePairs.Find(name);
	*value = "30.0; 0.001; 0.0";
	if (value)
		addOptionToStorage(name, *value);

	addConstraintButtonOk();
	addConstraintButtonEsc();
}




bool UExSimMainWidget::Initialize()
{
	updateDebugText(std::string("test"));	
	/*if (DebugText)
	{
		UE_LOG(LogTemp, Warning, TEXT("Debug text is inited"));
		std::string str = "Debug Info";
		DebugText->SetText(FText::FromString(str.c_str()));
	}*/
	if (SwitchObjButton)
		SwitchObjButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onSwitchObjButtonClicked);

	if(ApplyConstrButton)
	{
		//ApplyConstrButton->SetVisibility(ESlateVisibility::Hidden);
		ApplyConstrButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onApplyConstrButtonClicked);
	}
	
	//this->GetOwningPlayer()->Player->GetPlayerController(this->GetWorld())->bShowMouseCursor = true;
	if (InputOptions)
		InputOptions->SetText(FText::FromString("Hello"));

	if (DataStorage)
	{
		std::string * str = DataStorage->GenObjType.Find(GenObjKey++);
		updateSwitchObjText(*str);
		DataStorage->setTargetWidget(this);

		//GetOwningPlayer()->bShowMouseCursor = true;
		
	}

	

	

	if (GenerateButton)
	{
		GenerateButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onGenerateButtonClicked);
	}

	if (ChangeModeButton)
	{
		ChangeModeButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onChangeModeButtonClicked);
	}

	if (ConstrP2PButton&&ConstrGen6dofSpringButton&&ConstrHingeButton)
	{
		ConstrP2PButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onConstrP2PButtonClicked);
		ConstrGen6dofSpringButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onConstrGen6dofSpringButtonClicked);
		ConstrHingeButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onConstrHingeButtonClicked);
	}
	
	return Super::Initialize();
}
