// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimMainWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

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
	if (escButton)
		escButton->SetVisibility(value);
	if(ParentButton)
    	ParentButton->SetVisibility(value);
	if (TargetButton)
    	TargetButton->SetVisibility(value);
	
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

void UExSimMainWidget::onParentButtonClicked()
{
	//FString output = "On parent button click! ";
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, output);
	ParentActor = CurrentActor;
	if (ParentActor == TargetActor)
		TargetActor = nullptr;
	setVisibilityOptionsPanel(false);
}

void UExSimMainWidget::onTargetButtonClicked()
{
	TargetActor = CurrentActor;
	if (ParentActor == TargetActor)
		ParentActor = nullptr;
	
	setVisibilityOptionsPanel(false);
}

void UExSimMainWidget::onEscButtonClicked()
{
	setVisibilityOptionsPanel(false);
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
			CurrentActor = actor;
			FString pair_info = "Pair info:";
			pair_info += getParTrgInfo();
			pair_info += info;
			DebugText->SetText(FText::FromString(pair_info));
			if (ParentActor)
				ParentText->SetText(FText::FromString(TEXT("Parent: ") + ParentActor->GetName()));
			else
				ParentText->SetText(FText::FromString(TEXT("Parent: None")));
			
			if (TargetActor)
				TargetText->SetText(FText::FromString(TEXT("Target: ") + TargetActor->GetName()));
			else
				TargetText->SetText(FText::FromString(TEXT("Target: None")));
			
		}
	}
}

void UExSimMainWidget::onGenerateButtonClicked()
{
	if (DataStorage)
	{
		DataStorage->createSceneObj();
	}
}

void UExSimMainWidget::onChangeModeButtonClicked()
{
	if (ChangeModeText)
	{
		FString output = "On parent button click! ";
		output = DataStorage->switchMode();
     	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, output);
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

void UExSimMainWidget::addOptionToStorage(FString name, FString value)
{
	UExEditableWidget * Menu = CreateWidget<UExEditableWidget>(this, OptionClass);
	OptionsList.Add(Menu);
	Menu->ValueName->SetText(FText::FromString(name));
	Menu->ValueText->SetText(FText::FromString(value));
	StorageWrapBox->AddChild(Menu);
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

AActor* UExSimMainWidget::getParentActor()
{
	return ParentActor;
}

AActor* UExSimMainWidget::getTargetActor()
{
	return TargetActor;
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
		info = TEXT("\npar: ") + ParentActor->GetName();
	if (TargetActor)
		info += TEXT("\ntrg: ") + TargetActor->GetName();
	return info;
}

void UExSimMainWidget::onOptionsButtonOkClicked()
{
	OptionsButton_Ok->RemoveFromParent();
	//OptionsButton_Ok->FinishDestroy();
	//delete OptionsButton_Ok;
	StorageWrapBox->ClearChildren();
	for (auto & option : OptionsList)
		option->RemoveFromParent();
	OptionsList.Empty();
	for (auto & select : SelectorList)
		select->RemoveFromRoot();
	SelectorList.Empty();
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

	if (ParentButton)
	{
		ParentButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onParentButtonClicked);
		ParentButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (TargetButton)
	{
		TargetButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onTargetButtonClicked);
		TargetButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (escButton)
	{
		escButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onEscButtonClicked);
		escButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GenerateButton)
	{
		GenerateButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onGenerateButtonClicked);
	}

	if (ChangeModeButton)
	{
		ChangeModeButton->OnClicked.AddUniqueDynamic(this, &UExSimMainWidget::onChangeModeButtonClicked);
	}

	
	
	return Super::Initialize();
}
