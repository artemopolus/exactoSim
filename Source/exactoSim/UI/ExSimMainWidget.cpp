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

void UExSimMainWidget::updateDebugText(FString str)
{
	DebugText->SetText(FText::FromString(str));
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


static void sendDebug(FString text, FColor color = FColor::Green)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, text);
}

void UExSimMainWidget::setupConstrainOptions(FVector2D loc, AActor *actor)
{
	if (DataStorage)
	{
		FString info = "";
		if (DataStorage->touchActor(actor, info))
		{
			clearButtonTempList();
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
			// DebugText->SetText(FText::FromString(pair_info));



			FString out = TEXT("Constraint  pairs list:\n");
			auto cstr = *CurrentActor->getConstraints();
			for(int i = 0; i < cstr.Num(); i++)
			{
				const FString n = cstr[i]->getName();
				addButtonToTempList(n, i);
				out += FString::FromInt(i) + TEXT("\t ") + n + TEXT("\n");
			}
			sendDebug(out);
			addButtonToTempList(TEXT("Add new constraint"), -1);
			out = (ParentActor) ? (TEXT("Parent: ") + ParentActor->getName()) : TEXT("Parent: None");
			addButtonToTempList(out, -2);
			out = (TargetActor) ? (TEXT("Target: ") + TargetActor->getName()) : TEXT("Target: None");
			addButtonToTempList(out, -3);
			addButtonToTempList(TEXT("Edit component"), -4);
			
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

void UExSimMainWidget::setClickerClass(UClass* tmpl)
{
	ClickerClass = tmpl;
}


void UExSimMainWidget::onConstraintResetClicked()
{
}

void UExSimMainWidget::addButtonToStorage(FString name)
{
	//if (!OptionsButton_Ok)
	//{
		OptionsButton_Ok = CreateWidget<UExButtonWidget>(this, ButtonClass);
		StorageWrapBox->AddChild(OptionsButton_Ok);
		OptionsButton_Ok->setName(name);
		OptionsButton_Ok->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onTempButtonOkClicked);
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

void UExSimMainWidget::addTempButtonOk()
{
	addTempButtonOk(TEXT("ok"));
	OptionsButton_Ok->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onTempButtonOkClicked);
}

void UExSimMainWidget::addTempButtonDelete()
{
	addTempButtonOk(TEXT("Delete"));
	OptionsButton_Ok->ButtonBase->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onTempButtonDeleteClicked);
}

void UExSimMainWidget::addTempButtonOk(const FString name)
{
	UExButtonWidget * bt = CreateWidget<UExButtonWidget>(this, ButtonClass);
	StorageWrapBox->AddChild(bt);
	bt->setName(name);

	OptionsButton_Ok = bt;
}

void UExSimMainWidget::onTempListButtonClicked()
{
	for(int i = 0; i < ButtonTempList.Num(); i++)
	{
		if (ButtonTempList[i]->IsHovered())
		{
			sendDebug(ButtonTempList[i]->getButtonName());
			if (ButtonTempList[i]->tag == -1) //create new constraint
			{
				sendDebug(TEXT("Create constraint table"));
				createConstraintParamTable();
			}
			else if (ButtonTempList[i]->tag == -2)
			{
				setCurrentToParent();
			}
			else if (ButtonTempList[i]->tag == -3)
			{
				setCurrentToTarget();
			}
			else if (ButtonTempList[i]->tag == -4)
			{

				editComponentParamTable();
			}
			else if (ButtonTempList[i]->tag  > -1)
			{
				const auto cstr = *CurrentActor->getConstraints();
				auto * constraint = cstr[ButtonTempList[i]->tag];
				CurrentConstraint = constraint;
				editConstraintParamTable();
				// DataStorage->setOptVPP(constraint);
				
				// resetTemporary();
				// getInputTableOptions();
				// addInputTable();
			}
		}
	}
	updateExSmWidget();
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
	return ParentActor->getTarget();
}

AActor* UExSimMainWidget::getTargetActor()
{
	return TargetActor->getTarget();
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


void UExSimMainWidget::setCurrentToParent()
{
	DataStorage->setConstraintParentCommand(CurrentActor);
	ParentActor = CurrentActor;
	if (ParentActor == TargetActor)
		TargetActor = nullptr;
}

void UExSimMainWidget::setCurrentToTarget()
{
	DataStorage->setConstraintTargetCommand(CurrentActor);
	TargetActor = CurrentActor;
	if (ParentActor == TargetActor)
		ParentActor = nullptr;
}


bool UExSimMainWidget::getVectorFromString(FString list, FString splitter, FVector& out)
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


bool UExSimMainWidget::checkBoolArrayOption(UExEditableWidget* option, EnExConstraintParamNames checker,
                                            TArray<bool>& vect)
{
	if (option->ValueName->GetText().ToString() == DataStorage->OptionNamesPtr[checker])
	{
		TArray<FString> values;
		FString list = option->ValueText->GetText().ToString();
		list.ParseIntoArray(values, TEXT(";"), true);
		for (auto& val : values)
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

bool UExSimMainWidget::checkStringOption(UExEditableWidget* option, EnExConstraintParamNames checker,
                                         FString& name)
{
	if (option->getName() == DataStorage->OptionNamesPtr.FindRef( checker ))
	{
		name = option->getValue();
		return true;
	}
	return false;	
}

bool UExSimMainWidget::checkVectorOption(UExEditableWidget * option, EnExConstraintParamNames checker, FVector & vect)
{
	if (option->getName() == DataStorage->OptionNamesPtr[ checker ])
	{
		getVectorFromString(option->getValue(), TEXT(";"),vect);
		return true;
	}
	return false;
}

void UExSimMainWidget::onTempButtonOkClicked()
{
	//delete table of constraint option 
	resetTemporary();
	DataStorage->createCommand();
}

void UExSimMainWidget::onTempButtonDeleteClicked()
{
	resetTemporary();
	DataStorage->deleteCommand();
}


void UExSimMainWidget::onConstraintEscClicked()
{
	resetTemporary();
}

void UExSimMainWidget::addInputTable(TArray<AExSimStorage::ParamHolder> options)
{
	resetTemporary();
	for (const auto option : options)
	{
		if (option.EditType == EnExParamEdit::EDITABLE)
			addEditableToStorageWB(option.Name, option.Value[0], 0, option.ParamType);
		else if (option.EditType == EnExParamEdit::SELECTABLE)
			addSelectableToStorageWB(option.Name, option.Value, 0, option.ParamType);
		else if (option.EditType == EnExParamEdit::CLICKABLE)
		{
			addClickerToStorageWB(option.Name, option.Value[0], 0, option.ParamType);
		}
	}

}

void UExSimMainWidget::onCreateComponentClicked()
{
	sendDebug(TEXT("Create component"));
	createComponentParamTable();
}

void UExSimMainWidget::getInputTableOptions()
{
	if (!OptionNames)
		OptionNames = &DataStorage->OptionNamesPtr;
	if (!OptionValuePairs)
		OptionValuePairs = &DataStorage->OptionValuePairsPtr;
    
}
void UExSimMainWidget::addOptionToTable( TMap<int, FString> names, TMap<FString, FString > values, int id)
{
	if (EditableList.Num())
		clearOptionFromTable();
	for (const auto& option : names)
	{
		FString name = option.Value;
		const int type = option.Key;
		const FString* value = values.Find(name);
		if (value)
			addEditableToStorageWB(name, *value, id, type);
	}
	addTempButtonOk();
	addConstraintButtonEsc();
}
void UExSimMainWidget::addOptionToTable()
{
	if (EditableList.Num())
	{
		clearOptionFromTable();	
	}
	for (TTuple<EnExConstraintParamNames, FString>  option : DataStorage->OptionNamesPtr)
	{
		FString name = option.Value;
		int type = static_cast<int>(option.Key);
		FString * value = OptionValuePairs->Find(name);
		if (value)
			addEditableToStorageWB(name, *value,0,type);
	}
}


void UExSimMainWidget::addInputTable()
{
	UExSelector* sl = CreateWidget<UExSelector>(this, SelectorClass);
	sl->init(TEXT("Constraint: "), 0, 0);
	for (int i = 0; i < static_cast<int>(ExSimPhyzHelpers::NONE); i++)
		sl->addSelectorValue(ExSimPhyzHelpers::getNameFromConstraint(static_cast<ExSimPhyzHelpers::Constraint>(i)));
	sl->EventOnSelectorValueChanged.AddDynamic(this, &UExSimMainWidget::onSelectorWidgetChanged);
	SelectorList.Add(sl);
	StorageWrapBox->AddChild(sl);


	addOptionToTable();
	addTempButtonOk();
	addConstraintButtonEsc();
}

void UExSimMainWidget::addSelectableToStorageWB(FString name, TArray<FString> value, int id, int type)
{
	UExSelector * sl = CreateWidget<UExSelector>(this, SelectorClass);
	sl->init(name, id, type);
	for(int32 i = 0; i < value.Num(); i++)
		sl->addSelectorValue(value[i]);
	sl->EventOnSelectorValueChanged.AddDynamic(this, &UExSimMainWidget::onSelectorWidgetChanged);
	SelectorList.Add(sl);
	StorageWrapBox->AddChild(sl);
}
void UExSimMainWidget::addEditableToStorageWB(FString name, FString value, int id, int type)
{
	UExEditableWidget * Menu = CreateWidget<UExEditableWidget>(this, OptionClass);
	Menu->initEditable( name, value, id,type);
	Menu->EventOnTextCommit.AddDynamic(this, &UExSimMainWidget::onEditableWidgetChanged);
	EditableList.Add(Menu);
	StorageWrapBox->AddChild(Menu);
}
void UExSimMainWidget::addClickerToStorageWB(FString name, FString value, int id, int type)
{
	UExClickerWidget * menu = CreateWidget<UExClickerWidget>(this, ClickerClass);
	ClickerList.Add(menu);
	StorageWrapBox->AddChild(menu);
}
void UExSimMainWidget::updateEditable(EnExConstraintParamNames type, FString value)
{
	for(const auto & op : EditableList)
	{
		if (op->getType() == static_cast<int>(type))
		{
			op->update(value);
		}
	}
}

void UExSimMainWidget::updateEditable(EnExConstraintParamNames type, FVector value)
{
	FString v = ExConvert::getStrFromVec(value);
	updateEditable(type, v);
}



void UExSimMainWidget::onEditableWidgetChanged(FString ini, FString gen, int id, int type)
{
	sendDebug(TEXT("Change is accepted: ") + ini + TEXT(" => ") + gen);
	DataStorage->updateCommand(type, gen);
}
void UExSimMainWidget::onSelectorWidgetChanged(FString init, FString value, int32 type, int32 id, int32 eventtype)
{
	sendDebug("SELECTOR is CHANGED");
	DataStorage->updateCommand(type, value);
}
void UExSimMainWidget::onCommanderUpdated(FExCommonParams * params)
{

	updateEditable(params);
}


void UExSimMainWidget::updateEditable(FExCommonParams* params)
{
	// if (params->getType() != CurrentTableType)
	// {
	// 	if (params->isConstraint())
	// 	{
	// 		CurrentConstraint = static_cast<ExSimConstraintPair*>(params->getOwner());
	// 		editConstraintParamTable();
	// 	}
	// 	else if(params->isComponent())
	// 	{
	// 		CurrentActor = static_cast<ExSimComponent*>(params->getOwner());
	// 		editComponentParamTable();
	// 	}
	// 	return;
	// }
	TArray<AExSimStorage::ParamHolder> options;
	DataStorage->updateCommand(params,&options);
	clearOptionFromTable();
	addInputTable(options);
	addTempButtonOk();
	addConstraintButtonEsc();

	// for (auto& opt : options)
	// {
	// 	if(opt.EditType == EnExParamEdit::EDITABLE)
	// 	{
	// 		for (auto& ed : EditableList)
	// 		{
	// 			if (opt.ParamType == ed->getType())
	// 				ed->update(opt.Value[0]);
	// 		}
	// 	}
	// 	else if(opt.EditType == EnExParamEdit::SELECTABLE)
	// 	{
	// 		for(auto & sel : SelectorList)
	// 		{
	// 			if(opt.ParamType == sel->getType())
	// 				sel->setCurrentValue(opt.Value[0]);
	// 		}
	// 	}
	// }
}

void UExSimMainWidget::updateEditableAll()
{
	DataStorage->updateOptVPP();
	getInputTableOptions();
	for (auto & ed : EditableList)
	{
		FString name = ed->getName();
		auto v = DataStorage->OptionValuePairsPtr.FindRef(ed->getName());
		ed->update(*v);
	}
}

void UExSimMainWidget::onLoadClicked()
{
	DataStorage->loadComplex();
}

void UExSimMainWidget::onSaveClicked()
{
	if (ParentActor)
	{
		DataStorage->selectComplex(ParentActor);
		DataStorage->saveComplex();
	}
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


	if (DataStorage)
	{
		std::string* str = DataStorage->GenObjType.Find(GenObjKey++);
		DataStorage->setTargetWidget(this);
		// DataStorage->EssEvOnConstraintChanged.AddDynamic(this, &UExSimMainWidget::onDataStorageConstraintChanged);

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

	if (SaveButton && LoadButton && CreateComponentButton)
	{
		SaveButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onSaveClicked);
		LoadButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onLoadClicked);
		CreateComponentButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onCreateComponentClicked);
	}

	return Super::Initialize();
}

void UExSimMainWidget::initSimMainWidget()
{
	if (DataStorage)
	{
		DataStorage->setTargetWidget(this);

		// DataStorage->EssEvOnConstraintChanged.AddDynamic(this, &UExSimMainWidget::onDataStorageConstraintChanged);

		DataStorage->getCommander()->EcEvOnCommand.AddUObject(this, &UExSimMainWidget::onCommanderUpdated);

		onChangeModeButtonClicked();
	}
}

void UExSimMainWidget::updateExSmWidget()
{
	FString out = TEXT("Selected:\n");
	if (ParentActor)
		out += TEXT("Parent: ") + ParentActor->getName() + TEXT("\nComplex: ") + ParentActor->getComplexName() +
			TEXT("\n");
	if (TargetActor)
		out += TEXT("Target: ") + TargetActor->getName() + TEXT("\n");

	updateDebugText(out);

	clearButtonTempList();
}

void UExSimMainWidget::clearOptionFromTable()
{
	for (auto& option : EditableList)
		option->RemoveFromParent(); // don't delete widget directly
	EditableList.Empty();
	for (auto& select : SelectorList)
		select->RemoveFromParent();
	SelectorList.Empty();
	for (auto& select : ClickerList)
		select->RemoveFromParent();
	ClickerList.Empty();
}

void UExSimMainWidget::resetTemporary()
{
	if (OptionsButton_Ok)
	{
		OptionsButton_Ok->RemoveFromParent();
	}
	if (OptionsButton_Esc)
	{
		OptionsButton_Esc->RemoveFromParent();
	}
	clearOptionFromTable();

	StorageWrapBox->ClearChildren();
}

void UExSimMainWidget::createComponentParamTable()
{
	resetTemporary();
	TArray<AExSimStorage::ParamHolder> options;
	DataStorage->initComponentCommand(&options);
	addInputTable(options);
	addTempButtonOk();
	addConstraintButtonEsc();
	CurrentTableType = EnExParamTypes::COMPONENT;
}

void UExSimMainWidget::createConstraintParamTable()
{
	resetTemporary();
	TArray<AExSimStorage::ParamHolder> options;
	DataStorage->initConstraintCommand(&options);
	addInputTable(options);
	addTempButtonOk();
	addConstraintButtonEsc();
	CurrentTableType = EnExParamTypes::CONSTRAINT;
}

void UExSimMainWidget::editComponentParamTable()
{
	resetTemporary();
	TArray<AExSimStorage::ParamHolder> holder;
	DataStorage->selectCommand(CurrentActor, &holder);
	addInputTable(holder);
	addTempButtonDelete();
	addConstraintButtonEsc();
	CurrentTableType = EnExParamTypes::COMPONENT;
}

void UExSimMainWidget::editConstraintParamTable()
{
	resetTemporary();
	TArray<AExSimStorage::ParamHolder> holder;
	DataStorage->selectCommand(CurrentConstraint, &holder);
	addInputTable(holder);
	addTempButtonDelete();
	addConstraintButtonEsc();
	CurrentTableType = EnExParamTypes::CONSTRAINT;
	
}
