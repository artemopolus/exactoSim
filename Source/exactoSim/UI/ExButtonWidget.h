// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API UExButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * ButtonBase;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ButtonName;

	UFUNCTION(BlueprintCallable)
		void setName(FString name);
	UFUNCTION(BlueprintCallable)
		FString getButtonName();

	int tag = -1;
private:
	FString Name;
};
