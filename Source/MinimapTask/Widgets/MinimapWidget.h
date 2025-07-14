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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Minimap")
	void RegisterTrackedActor(AActor* ActorToTrack, TSubclassOf<UMinimapIconWidget> IconClass);

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* IconContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float MapScale = 0.1f; // World units to minimap units

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	AActor* PlayerActor;

	UPROPERTY()
	TArray<FMinimapEntry> TrackedActors;

	FVector2D WorldToMinimapPosition(const FVector& WorldLocation) const;
};
