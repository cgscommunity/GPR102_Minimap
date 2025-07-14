// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapIconWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class MINIMAPTASK_API UMinimapIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void UpdateIcon(const FVector& PlayerLocation, const FVector& WorldLocation);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* IconImage;
};
