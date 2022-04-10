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
	UFUNCTION()
		void onSwitchObjButtonClicked();
	UFUNCTION()
		void onApplyConstrButtonClicked();

	UFUNCTION()
		void onParentButtonClicked();
	UFUNCTION()
		void onTargetButtonClicked();
	UFUNCTION()
		void onEscButtonClicked();

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
		void onConstrHingeButtonClicked();
	UFUNCTION()
		void onConstrGen6dofSpringButtonClicked();
	UFUNCTION()
		void onConstrP2PButtonClicked();	
	virtual ~UExSimMainWidget() override;

	
	void setSelectorClass(UClass * tmpl);
	void setButtonClass(UClass * tmpl);
	void setOptionClass(UClass * tmpl);

	void addOptionToStorage(FString name, FString value);
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
	void setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha);
	void drawPtOnCanvas(int32 x, int32 y, uint8 red, uint8 green, uint8 blue, uint8 alpha);
	void updateDebugText(const std::string str);
	void updateSwitchObjText(const std::string str);
	void updateSwitchObjText(const FString str);
	void setVisibilityOptionsPanel(bool onoff);

	bool getVectorFromString(FString list, FString splitter, FVector & out);

	void deleteConstraintOptions();

	void addInputTable();
	void addButtonToTempList(const FString name, const int tag);
	void clearButtonTempList();
	
protected:
	bool Initialize() override;
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock * DebugText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D * RawDataOutput;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * SwitchObjButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * SwitchObjText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * ApplyConstrButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ApplyConstrText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * ParentButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ParentText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * TargetButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * TargetText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * escButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * escText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox * InputOptions;

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
		class UButton * ConstrHingeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ConstrHingeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * ConstrGen6dofSpringButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ConstrGen6dofSpringText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton * ConstrP2PButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ConstrP2PText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWrapBox * StorageWrapBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWrapBox * TempWrapBox;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExSimStorage * DataStorage;



	TMap<AExactoPhysics::es_options_list, FString> OptionNames;
	TMap<FString, FString> OptionValuePairs;
	

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
	int ConstrKey = BulletHelpers::Constr::NONE;

	
	
	TArray<UExEditableWidget *> OptionsList;
	TArray<UExSelector *> SelectorList;
	TArray<UExButtonWidget *> ButtonTempList;
	UExButtonWidget * OptionsButton_Ok = nullptr;
	UExButtonWidget * OptionsButton_Esc = nullptr;
	UExButtonWidget * OptionsButton_Reset = nullptr;

	UClass * SelectorClass;
	UClass * ButtonClass;
	UClass * OptionClass;

	AActor * CurrentActor;
	TArray<AExSimStorage::es_constraint_pair *> ConstrPairList;
	
	AActor * TargetActor;
	AActor * ParentActor;
	BulletHelpers::Constr SelectedConstraintType = BulletHelpers::Constr::NONE;
	
	bool checkVectorOption(UExEditableWidget * option, AExactoPhysics::es_options_list checker, FVector & vect);
	bool checkBoolArrayOption(UExEditableWidget * option, AExactoPhysics::es_options_list checker, TArray<bool> & vect);
	bool checkStringOption(UExEditableWidget * option, AExactoPhysics::es_options_list checker, FString & name);
	
};
