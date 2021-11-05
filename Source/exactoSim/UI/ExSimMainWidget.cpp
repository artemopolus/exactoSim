// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimMainWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

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

void UExSimMainWidget::onSwitchObjButtonClicked()
{
	std::string * str = nullptr;
	while(str == nullptr)
	{
		str = DataStorage->GenObjType.Find(++GenObjKey);
		if (GenObjKey >= AExSimStorage::exsim_genobj_type::EXGT_END)
			GenObjKey = -1;
	}
	updateSwitchObjText(*str);
	if (DataStorage)
		DataStorage->registerExtendedCmd(AExSimStorage::exsim_cmd_type::EXCT_SWITCH, GenObjKey);
		//DataStorage->registerCmdToSelected(AExSimStorage::exsim_cmd_type::EXCT_SWITCH, static_cast<float>(GenObjKey - 1));
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
	
	//this->GetOwningPlayer()->Player->GetPlayerController(this->GetWorld())->bShowMouseCursor = true;

	if (DataStorage)
	{
		std::string * str = DataStorage->GenObjType.Find(GenObjKey++);
		updateSwitchObjText(*str);
	}
	
	return Super::Initialize();
}
