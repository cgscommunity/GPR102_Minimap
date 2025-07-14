// © 2025 CG Spectrum. All Rights Reserved.

// This file's header
#include "MinimapWidget.h"

// Other includes
#include "MinimapIconWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

void UMinimapWidget::RegisterTrackedActor(AActor* ActorToTrack, TSubclassOf<UMinimapIconWidget> IconClass)
{
	if (!ActorToTrack || !IconContainer || !IconClass) return;

	UMinimapIconWidget* Icon = CreateWidget<UMinimapIconWidget>(this, IconClass);
	if (!Icon) return;

	IconContainer->AddChild(Icon);

	FMinimapEntry Entry;
	Entry.TrackedActor = ActorToTrack;
	Entry.IconWidget = Icon;

	TrackedActors.Add(Entry);
}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!PlayerActor) return;

	FVector PlayerLocation = PlayerActor->GetActorLocation();

	for (int32 i = TrackedActors.Num() - 1; i >= 0; --i)
	{
		if (!TrackedActors[i].TrackedActor || !TrackedActors[i].IconWidget)
		{
			TrackedActors.RemoveAt(i);
			continue;
		}

		FVector TargetLocation = TrackedActors[i].TrackedActor->GetActorLocation();
		FVector RelativeLocation = TargetLocation - PlayerLocation;

		FVector2D MinimapPos = WorldToMinimapPosition(RelativeLocation);

		UCanvasPanelSlot* IconSlot = Cast<UCanvasPanelSlot>(TrackedActors[i].IconWidget->Slot);
		if (IconSlot)
		{
			IconSlot->SetPosition(MinimapPos);
		}
	}
}

FVector2D UMinimapWidget::WorldToMinimapPosition(const FVector& WorldOffset) const
{
	// X → Right, Y → Up (Unreal world Z is ignored here)
	return FVector2D(WorldOffset.X, WorldOffset.Y) * MapScale;
}