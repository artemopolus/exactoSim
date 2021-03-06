// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "SlateCore/Public/Types/SlateEnums.h"
#include "ExEditableWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API UExEditableWidget : public UUserWidget
{
	GENERATED_BODY()
public: //variables
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOptionParamChanged, FString, ini, FString, gen, int, id, int, type);
	FOptionParamChanged EventOnTextCommit;	
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ValueName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableText * ValueText;

private:
	int PtId = -1;
	int PtType = -1;
	FString InitValue = "";
public: //functions
	UFUNCTION()
		void onTextCommitedRegistered(const FText & text, ETextCommit::Type type);
		
	void initEditable(FString name, FString value, int id, int type);
	void update(FString value);
	int getId() const {return PtId;}
	int getType() const {return PtType;}
	FString getName() const;
	FString getValue() const;
};
