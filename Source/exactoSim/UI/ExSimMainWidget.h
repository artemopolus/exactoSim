// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <string>
#include "CoreMinimal.h"
#include "ExButtonWidget.h"
#include "ExEditableWidget.h"
#include "ExSelector.h"
#include "Blueprint/UserWidget.h"
#include "exactoSim/Common/ExSimStorage.h"
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

	UFUNCTION(BlueprintCallable, Category= Common)
		void initSimMainWidget();
	UFUNCTION(BlueprintCallable, Category=Common)
		void updateExSmWidget();
	


	UFUNCTION()
		void setupConstrainOptions(FVector2D loc, AActor *actor);

	UFUNCTION()
		void onGenerateButtonClicked();

	UFUNCTION()
		void onChangeModeButtonClicked();

	UFUNCTION()
		void onOptionsButtonOkClicked();

	UFUNCTION()
		void onTempListButtonClicked();

	UFUNCTION()
		void onConstraintEscClicked();

	UFUNCTION()
		void onConstraintResetClicked();
	
	UFUNCTION()
		void onCreateComponentClicked();
	UFUNCTION()
		void onLoadClicked();
	UFUNCTION()
		void onSaveClicked();

	UFUNCTION()
		void onEditableWidgetChanged(FString ini, FString gen, int id, int type);
	UFUNCTION()
		void onDataStorageConstraintChanged(int type, FString value);
	UFUNCTION()
		void onSelectorWidgetChanged(FString value, ESelectInfo::Type type, int id);

	
	
	virtual ~UExSimMainWidget() override;

	
	void setSelectorClass(UClass * tmpl);
	void setButtonClass(UClass * tmpl);
	void setOptionClass(UClass * tmpl);
	void setComboClass(UClass * tmpl);

	void addEditableToStorageWB(FString name, FString value, int id, int type);
	void updateEditable(EConstraintParamNames type, FString value);
	void updateEditable(EConstraintParamNames type, FVector value);
	void updateEditableAll();
	void addButtonToStorage(FString name);
	void addSelectToStorage(FString name, TArray<FString> option_list);

	void addConstraintButtonOk();
	void addConstraintButtonEsc();
	void addConstraintButtonReset();

	AActor * getParentActor();
	AActor * getTargetActor();
	bool isParTrgPair();
	FString getParTrgInfo();


	
private:
	void setCurrentToParent();
	void setCurrentToTarget();
	void getInputTableOptions();
	
	void setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha);
	void drawPtOnCanvas(int32 x, int32 y, uint8 red, uint8 green, uint8 blue, uint8 alpha);
	void updateDebugText(const std::string str);
	void updateDebugText(FString str);
	void setVisibilityOptionsPanel(bool onoff);

	bool getVectorFromString(FString list, FString splitter, FVector & out);

	void deleteConstraintOptions();

	void addOptionToTable();
	void clearOptionFromTable();
	void addInputTable();
	
	void addButtonToTempList(const FString name, const int tag);
	void clearButtonTempList();
	bool checkVectorOption(UExEditableWidget * option, EConstraintParamNames checker, FVector & vect);
	bool checkBoolArrayOption(UExEditableWidget * option, EConstraintParamNames checker, TArray<bool> & vect);
	bool checkStringOption(UExEditableWidget * option, EConstraintParamNames checker, FString & name);
	
protected:
	bool Initialize() override;
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock * DebugText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D * RawDataOutput;
	







	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel * OptionsPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * GenerateButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * GenerateText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * ChangeModeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ChangeModeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * CreateComponentButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * CreateComponentText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * LoadButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * LoadText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * SaveButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * SaveText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWrapBox * StorageWrapBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWrapBox * TempWrapBox;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExSimStorage * DataStorage;



	TMap<EConstraintParamNames, FString> * OptionNames;
	TMap<FString, FString> * OptionValuePairs;
	

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

	int GenObjKey = 0;
	int ConstrKey = ExSimPhyzHelpers::Constraint::NONE;

	
	
	TArray<UExEditableWidget *> EditableList;
	TArray<UExSelector *> SelectorList;
	TArray<UExButtonWidget *> ButtonTempList;
	UExButtonWidget * OptionsButton_Ok = nullptr;
	UExButtonWidget * OptionsButton_Esc = nullptr;
	UExButtonWidget * OptionsButton_Reset = nullptr;

	UClass * SelectorClass;
	UClass * ButtonClass;
	UClass * OptionClass;
	UClass * ComboClass;

	ExSimComponent * CurrentActor;
	ExSimConstraintPair * CurrentConstraint;
	TArray<ExSimConstraintPair *> ConstrPairList;
	
	ExSimComponent * TargetActor;
	ExSimComponent * ParentActor;
	ExSimPhyzHelpers::Constraint SelectedConstraintType = ExSimPhyzHelpers::Constraint::NONE;

	
		

	
};
