// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CanvasItem.h"
#include "UObject/Object.h"
#include "ExUIoutput.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class EXACTOSIM_API UExUIoutput : public UObject
{
	GENERATED_BODY()

public:
	UExUIoutput();
	~UExUIoutput();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCanvas * DebugCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFont * DebugFont;
	UFUNCTION(BlueprintCallable)
		void updateText();
private:
	//FCanvasTextItem DebugTextItem;
};
