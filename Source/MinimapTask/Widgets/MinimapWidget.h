// © 2025 CG Spectrum. All Rights Reserved.

#pragma once

// Includes
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

// Forward declarations
class UCanvasPanel;

USTRUCT()
struct FMinimapEntry
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* TrackedActor = nullptr;

	UPROPERTY()
	UMinimapIconWidget* IconWidget = nullptr;
};

/**
 * 
 */
UCLASS()
class MINIMAPTASK_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	void RegisterTrackedActor(AActor* ActorToTrack, TSubclassOf<UMinimapIconWidget> IconClass);

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* IconContainer;

	UPROPERTY(meta = (BindWidget))
	UMinimapIconWidget* PlayerIconWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	TSubclassOf<UMinimapIconWidget> EnemyIconClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	TSubclassOf<UMinimapIconWidget> PlayerIconClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float MapScaleMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float MaxDistance = 1200.0f;

	UPROPERTY()
	TArray<FMinimapEntry> MinimapEntries;
};
