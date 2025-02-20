// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/PlayerMainUIWidget.h"

#include "ProjectVS.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Character/Monster/MonsterWorldManager.h"
#include "ProjectVSGameMode.h"
#include "PlayGameState.h"
#include "Player/VSPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Ability/AttributeSet_Default.h"
#include "Ability/AttributeSet_Player.h"
#include "GameplayEffectTypes.h"
#include "Components/RichTextBlock.h"
#include "Ability/VSAbility.h"
#include "Ability/AbilityBookComponent.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

#include "Components/Image.h"

#include "Ability/AbilityDataManager.h"

#include "Blueprint/WidgetTree.h"


#define LOCTEXT_NAMESPACE "ProjectVS Widget"


UPlayerMainUIWidget::UPlayerMainUIWidget(const FObjectInitializer& Initializer) : Super(Initializer)
{
	
}

void UPlayerMainUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* PlayerWorld = GetWorld())
	{
		AGameModeBase* GameMode = PlayerWorld->GetAuthGameMode();
		APlayGameState* PlayGameState = GameMode->GetGameState<APlayGameState>();

		SetKillCount(0,0);

		PlayGameState->GetOnMonsterKillCountChangedDelegate().AddUObject(this, &UPlayerMainUIWidget::SetKillCount);
		PlayGameState->GetOnStageTimeChangedDelegate().AddUObject(this, &UPlayerMainUIWidget::SetStageTime);
	}

	if (AVSPlayerState* VSPlayerState = GetOwningPlayerState<AVSPlayerState>())
	{
		UAbilitySystemComponent* ASC = VSPlayerState->GetAbilitySystemComponent();
		
		bool bFound = false;
		float TempValue = 0;
		
		TempValue = ASC->GetGameplayAttributeValue(UAttributeSet_Default::GetLevelAttribute(), bFound);
		SetLevel(TempValue);

		TempValue = ASC->GetGameplayAttributeValue(UAttributeSet_Player::GetEXPForLevelUpAttribute(), bFound);
		SetEXPForLevelUp(TempValue);

		TempValue = ASC->GetGameplayAttributeValue(UAttributeSet_Player::GetCurrentEXPAttribute(), bFound);
		SetCurrentEXP(TempValue);

		TempValue = ASC->GetGameplayAttributeValue(UAttributeSet_Player::GetGoldAttribute(), bFound);
		SetGold(TempValue);

		ASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Default::GetLevelAttribute()).AddUObject(this, &UPlayerMainUIWidget::OnLevelChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Player::GetCurrentEXPAttribute()).AddUObject(this, &UPlayerMainUIWidget::OnCurrentEXPChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Player::GetEXPForLevelUpAttribute()).AddUObject(this, &UPlayerMainUIWidget::OnEXPForLevelUpChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Player::GetGoldAttribute()).AddUObject(this, &UPlayerMainUIWidget::OnGoldChanged);
	
		UAbilityBookComponent* PlayerAbilityBook = VSPlayerState->GetAbilityBookComponent();
		RegisterAbilityBook(PlayerAbilityBook);
	}
}

void UPlayerMainUIWidget::NativeDestruct()
{
	if (UWorld* PlayerWorld = GetWorld())
	{
		AGameModeBase* GameMode = PlayerWorld->GetAuthGameMode();
		APlayGameState* PlayGameState = GameMode->GetGameState<APlayGameState>();

		PlayGameState->GetOnMonsterKillCountChangedDelegate().Remove(KillCountHandle);
		PlayGameState->GetOnStageTimeChangedDelegate().Remove(StageTimeHandle);
	}

	Super::NativeDestruct();
}

void UPlayerMainUIWidget::RegisterAbilityBook(UAbilityBookComponent* NewBook)
{
	if(NewBook == WeakAbilityBook.Get()) return;

	UnregisterAbilityBook();

	WeakAbilityBook = NewBook;
	if (WeakAbilityBook.IsValid())
	{
		OnRegisterAbilityBook(WeakAbilityBook.Get());
	}
}

void UPlayerMainUIWidget::UnregisterAbilityBook()
{
	if (!WeakAbilityBook.IsValid()) return;
	UAbilityBookComponent* PrevBook = WeakAbilityBook.Get();
	WeakAbilityBook.Reset();

	
	OnUnregisterAbilityBook(PrevBook);
}

void UPlayerMainUIWidget::OnRegisterAbilityBook(UAbilityBookComponent* NewBook)
{
	check(NewBook)
	UpdateIconBoxs();
	NewBook->GetOnUpdateBookDelegate().AddDynamic(this, &UPlayerMainUIWidget::OnUpdateAbilityBook);
}

void UPlayerMainUIWidget::OnUnregisterAbilityBook(UAbilityBookComponent* PrevBook)
{
	

	PrevBook->GetOnUpdateBookDelegate().RemoveDynamic(this, &UPlayerMainUIWidget::OnUpdateAbilityBook);
}

void UPlayerMainUIWidget::OnUpdateAbilityBook(UAbilityBookComponent* InBook)
{
	UpdateIconBoxs();
}




void UPlayerMainUIWidget::SetCurrentEXP(float NewEXP)
{
	if (NewEXP == CurrentEXP) return;

	CurrentEXP = NewEXP;
	UpdateEXPBar();
}

void UPlayerMainUIWidget::SetEXPForLevelUp(float NewEXPForLevelUp)
{
	if(NewEXPForLevelUp == EXPForLevelUp) return;

	EXPForLevelUp = NewEXPForLevelUp;
	UpdateEXPBar();
}

void UPlayerMainUIWidget::OnCurrentEXPChanged(const FOnAttributeChangeData& ChangedData)
{
	SetCurrentEXP(ChangedData.NewValue);
}

void UPlayerMainUIWidget::OnEXPForLevelUpChanged(const FOnAttributeChangeData& ChangedData)
{
	SetEXPForLevelUp(ChangedData.NewValue);
}

void UPlayerMainUIWidget::UpdateEXPBar()
{
	if (EXPForLevelUp <= 0 || EXPForLevelUp <= CurrentEXP)
	{
		EXPBar->SetPercent(1);
	}
	else if (CurrentEXP <= 0)
	{
		EXPBar->SetPercent(0);
	}
	else
	{
		float NewPercet = CurrentEXP / EXPForLevelUp;
		EXPBar->SetPercent(NewPercet);
	}
}

void UPlayerMainUIWidget::OnLevelChanged(const FOnAttributeChangeData& ChangedData)
{
	SetLevel(ChangedData.NewValue);
}

void UPlayerMainUIWidget::OnGoldChanged(const FOnAttributeChangeData& ChangedData)
{
	SetGold(ChangedData.NewValue);
}

void UPlayerMainUIWidget::SetGold(int NewGold)
{
	FString TempString = FString::Printf(TEXT("Gold : %d"), NewGold);
	GoldText->SetText(FText::FromString(TempString));
}

void UPlayerMainUIWidget::SetKillCount(int OldCount, int NewCount)
{
	FFormatNamedArguments Args;
	Args.Add("Count", NewCount);

	KillCountText->SetText(FText::Format(LOCTEXT("Kill Count String", "Kill Count : {Count}"), Args));
}

void UPlayerMainUIWidget::SetStageTime(float OldTime, float NewTime)
{
	FFormatNamedArguments Args;

	int NewMinute = NewTime / 60;
	int NewSeconds = NewTime - (NewMinute * 60);
	
	FString NewMinuteString = NewMinute < 10 ? FString::Printf(TEXT("0%d"), NewMinute) : FString::FromInt(NewMinute);
	FString NewSecondsString = NewSeconds < 10 ? FString::Printf(TEXT("0%d"),NewSeconds) : FString::FromInt(NewSeconds);

	FString NewStageTime = FString::Printf(TEXT("%s : %s"), *NewMinuteString, *NewSecondsString);
	StageTimeText->SetText(FText::FromString(NewStageTime));
}

void UPlayerMainUIWidget::SetLevel(int NewLevel)
{
	FString NewLevelString = FString::Printf(TEXT("Lv.%d"), NewLevel);
	LevelText->SetText(FText::FromString(NewLevelString));
}



void UPlayerMainUIWidget::UpdateIconBoxs()
{
	LOG_ERROR(TEXT("Test"));
	if(!WeakAbilityBook.IsValid()) {return;}

	TArray<const FAbilityPage*> ActivePages;
	WeakAbilityBook->GetAbilityPageArray(ActivePages, EVSAbilityType::Active);
	UpdateIconBox(ActiveIconBox, ActivePages);

	LOG_ERROR(TEXT("%d"), ActiveIconBox->GetChildrenCount());
	
	TArray<const FAbilityPage*> PassivePages;
	WeakAbilityBook->GetAbilityPageArray(PassivePages, EVSAbilityType::Passive);
	UpdateIconBox(PassiveIconBox, PassivePages);
}

void UPlayerMainUIWidget::UpdateIconBox(UHorizontalBox* IconBox, TArray<const FAbilityPage*>& Pages)
{
	LOG_ERROR(TEXT("Test"));

	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();
	check(ADM)

	int PageCount = Pages.Num();
	TArray<UWidget*> TempIcons = IconBox->GetAllChildren();
	for (UWidget*& TempWidget : TempIcons)
	{
		UImage* TempImage = StaticCast<UImage*>(TempWidget);
		TempImage->SetBrushFromTexture(nullptr);
	}

	for (int i = 0; i < PageCount; i++)
	{
		UImage* TargetImage = nullptr;
		if (TempIcons.IsValidIndex(i))
		{
			TargetImage = StaticCast<UImage*>(TempIcons[i]);
		}
		else
		{
			TargetImage = WidgetTree->ConstructWidget<UImage>();
			UVerticalBoxSlot* NewBoxSlot = StaticCast<UVerticalBoxSlot*>(IconBox->AddChild(TargetImage));
			NewBoxSlot->SetPadding(FMargin(Padding_Horizontal, Padding_Vertical));
		}

		FName AbilityCode = Pages[i]->GetAbilityCode();
		const UVSAbility* TempAbility = ADM->FindAbility(AbilityCode).GetDefaultObject();
		if (TempAbility)
		{
			TargetImage->SetBrushFromTexture(TempAbility->GetAbilityIcon());
		}

	}
}

#undef LOCTEXT_NAMESPACE