// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "ExSelector.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API UExSelector : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString * SelectorCombo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * SelectorText;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FEvSelectorValueChanged, FString, value, FString, gen, int32, type, int32, id, int32, eventtype);
	FEvSelectorValueChanged EventOnSelectorValueChanged;
private:
	int PtId = -1;
	int PtType = -1;
	FString InitValue;

public:

	void setSelectorText(FString name);
	void addSelectorValue(FString text);
	void setCurrentValue(FString trg);
	void init(FString name, int32 id, int32 type);
	UFUNCTION()
		void onSelectorValueChanged(FString name, ESelectInfo::Type type);
	int getId() const {return PtId;}
	int getType() const {return PtType;}	
};
