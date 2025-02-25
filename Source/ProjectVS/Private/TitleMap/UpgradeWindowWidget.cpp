// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMap/UpgradeWindowWidget.h"
#include "TitleMap/UpgradeWidget.h"
#include "Player/Upgrade/PlayerUpgradeSubsystem.h"
#include "Player/Upgrade/UpgradeDataAsset.h"
#include "VSGameInstance.h"
#include "VSSaveGame.h"
#include "Player/Upgrade/UpgradeInfo.h"
#include "ProjectVS.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

#define LOCTEXT_NAMESPACE "ProjectVS Widget"

UUpgradeWindowWidget::UUpgradeWindowWidget(const FObjectInitializer& Initializer) : Super(Initializer)
{
	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::Visible);
}


void UUpgradeWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UPlayerUpgradeSubsystem* PUSS = UPlayerUpgradeSubsystem::GetPlayerUpgradeSubsystem())
	{
		if (PUSS->IsLoadedUpgradeData())
		{
			InitializeUpgradeWidgets(PUSS);
		}
		else
		{
			PUSS->GetOnUpgradeDataLoadComplete().AddUObject(this, &UUpgradeWindowWidget::InitializeUpgradeWidgets);
		}
	}

	UpgradeButton->OnClicked.AddDynamic(this, &UUpgradeWindowWidget::OnUpgradeButtonClick);

	ExitButton->OnClicked.AddDynamic(this, &UUpgradeWindowWidget::ExitWindow);
}

FReply UUpgradeWindowWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}




void UUpgradeWindowWidget::InitializeUpgradeWidgets(UPlayerUpgradeSubsystem* InSystem)
{
	TArray<const UUpgradeDataAsset*> ResultAssets;
	InSystem->GetAllUpgradeAssets(ResultAssets);
	
	int R = 0;
	int C = 0;

	LOG_ERROR(TEXT("%d"), ResultAssets.Num());

	for (auto& Result : ResultAssets)
	{
		UUpgradeWidget* NewWidget = CreateWidget<UUpgradeWidget>(this, UpgradeWidgetClass);

		if (NewWidget)
		{
			NewWidget->SetUpgradeData(Result);

			NewWidget->SetWeakWindow(this);

			UUniformGridSlot* NewSlot = UpgradeWidgetPanel->AddChildToUniformGrid(NewWidget, R, C);

			if (MaxColumn <= ++C)
			{
				C = 0;
				++R;
			}

			NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			NewSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

		}
	}

	
}

void UUpgradeWindowWidget::OnFocusedUpgradeWidget(UUpgradeWidget* Target)
{
	if(Target == FocusedUpgrade.Get()) {return;}

	FocusedUpgrade = Target;

	RefreshFocusedWidget();
}

void UUpgradeWindowWidget::OnUpgradeButtonClick()
{
	if(!FocusedUpgrade.IsValid()) return;

	FocusedUpgrade->TryUpgrade();

	RefreshFocusedWidget();
}

void UUpgradeWindowWidget::RefreshFocusedWidget()
{
	bool bIsValid = FocusedUpgrade.IsValid();

	if (bIsValid)
	{
		//아이콘
		FocusedIcon->SetColorAndOpacity(FLinearColor::White);
		FocusedIcon->SetBrushFromTexture(FocusedUpgrade->GetIconImage());

		//업그레이드 이름
		FocusedName->SetText(FocusedUpgrade->GetNameText());

		//골드 텍스트
		FFormatNamedArguments Args;

		int UpgradeGold = FocusedUpgrade->GetUpgradeGold();

		Args.Add("Gold", UpgradeGold);

		FText GoldText = FText::Format(LOCTEXT("Upgrade Gold Text", "강화 비용 : {Gold}"), Args);

		UpgradeGoldText->SetText(GoldText);

		//업그레이드 버튼
		UVSGameInstance* VSInst = GetGameInstance<UVSGameInstance>();

		int CurrentGold = VSInst->GetGold();

		bool CanUpgrade = UpgradeGold <= CurrentGold;
		UpgradeButton->SetIsEnabled(CanUpgrade);

		DescriptionText->SetText(FocusedUpgrade->GetDescriptionText());
	}
	else
	{
		FocusedIcon->SetColorAndOpacity(FLinearColor::Black);

		FocusedName->SetText(FText::GetEmpty());

		UpgradeButton->SetIsEnabled(false);

		UpgradeGoldText->SetText(FText::GetEmpty());

		DescriptionText->SetText(FText::GetEmpty());
	}

	
}

void UUpgradeWindowWidget::ExitWindow()
{
	SetVisibility(ESlateVisibility::Collapsed);
}


#undef LOCTEXT_NAMESPACE