// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ExSimMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API UExSimMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void initializeCanvas(const int32 pixelsH, const int32 pixelsV);
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void updateCanvas();
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void clearCanvas();
	UFUNCTION(BlueprintCallable, Category = DrawingTools)
		void testDrawFunction();

private:
	void setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha);
	void drawPtOnCanvas(int32 x, int32 y, uint8 red, uint8 green, uint8 blue, uint8 alpha);
	
protected:
	bool Initialize() override;
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock * DebugText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D * RawDataOutput;
private:
	std::unique_ptr<uint8[]> CanvasPixelData;
	int CanvasWidth;
	int CanvasHeight;
	int BytesPerPixel;
	int BufferPitch;
	
	int BufferSize;
	std::unique_ptr<FUpdateTextureRegion2D> EchoUpdateTextureRegion;
	std::unique_ptr<uint8[]> canvasBrushMask;
	int radius;
	int brushBufferSize;	
};
