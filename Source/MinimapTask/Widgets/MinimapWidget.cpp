// © 2025 CG Spectrum. All Rights Reserved.

// This file's header
#include "MinimapWidget.h"

// Other includes
#include "MinimapIconWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MinimapTask/EnemyBot.h"

void UMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Register all enemies
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBot::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		RegisterTrackedActor(Actor, EnemyIconClass);
	}
}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

#pragma region NULLChecks
	// Get (and check) all the pointers we need
	if (!IconContainer)
	{
		UE_LOG(LogTemp, Display, TEXT("IconContainer is null"));
		return;
	}
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Display, TEXT("PlayerController is null"));
		return;
	}

	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Display, TEXT("PlayerPawn is null"));
		return;
	}

	UCameraComponent* Camera = PlayerPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera)
	{
		UE_LOG(LogTemp, Display, TEXT("CameraComponent is null"));
		return;
	}
#pragma endregion NULLChecks

	// Find the size and centre of our canvas
	const FVector2D CanvasSize = IconContainer->GetCachedGeometry().GetLocalSize();
	const FVector2D CanvasCenter = CanvasSize * 0.5f;

	// Get the player character location and camera rotation
	const FVector PlayerCharacterLocation = PlayerPawn->GetActorLocation();
	const FRotator CameraRotation = Camera->GetComponentRotation();

	// Get the camera's forward facign direction and zero our the Z (height)
	FVector CamForward = CameraRotation.Vector();
	CamForward.Z = 0.f;
	
	// Use cross product to figure out the camera's "right" vector
	const FVector CamRight = FVector::CrossProduct(FVector::UpVector, CamForward);

	// Calculate the conversation scale from world units (cm) to screen space (px) 
	const float Scale = (CanvasSize.X * MapScaleMultiplier) / MaxDistance;

	// Loop over each minimap entry
	for(int32 i = MinimapEntries.Num() - 1; i >= 0; --i)
	{
		FMinimapEntry& Entry = MinimapEntries[i];
		if (!Entry.TrackedActor || !Entry.IconWidget)
		{
			MinimapEntries.RemoveAt(i);
			continue;
		}

		// Get the vector offset from the player to the tracked actor and flatten the Z (height)
		FVector WorldOffset = Entry.TrackedActor->GetActorLocation() - PlayerCharacterLocation;
		WorldOffset.Z = 0.f;

		// Project the world offset into camera-relative local space
		const float ForwardAmount = FVector::DotProduct(WorldOffset, CamForward);
		const float RightAmount = FVector::DotProduct(WorldOffset, CamRight);

		// Create 2d offset for minimap (flipping y so "up" is "in front")
		FVector2D Offset2D(RightAmount, -ForwardAmount);

		// Clamp distance to the edge of the minimap's visible radius
		if (Offset2D.SizeSquared() > MaxDistance * MaxDistance)
		{
			Offset2D = Offset2D.GetSafeNormal() * MaxDistance;
			Entry.IconWidget->SetRenderOpacity(0.0f);
		}
		else
		{
			Entry.IconWidget->SetRenderOpacity(1.0f);
		}

		// Convert world position to final screen-space position on minimap
		const FVector2D FinalPos = CanvasCenter + Offset2D * Scale;

		// Update the associated canvas slot with the correct possition/size/alignment data
		if (UCanvasPanelSlot* IconSlot = Cast<UCanvasPanelSlot>(Entry.IconWidget->Slot))
		{
			IconSlot->SetPosition(FinalPos);
			IconSlot->SetSize(FVector2D(25.f, 25.f));
			IconSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		}
	}
}


void UMinimapWidget::RegisterTrackedActor(AActor* ActorToTrack, TSubclassOf<UMinimapIconWidget> IconClass)
{
	if (!ActorToTrack || !IconContainer || !IconClass) return;

	UMinimapIconWidget* Icon = CreateWidget<UMinimapIconWidget>(this, IconClass);
	if (!Icon) return;

	IconContainer->AddChild(Icon);

	FMinimapEntry Entry;
	Entry.TrackedActor = ActorToTrack;
	Entry.IconWidget = Icon;

	MinimapEntries.Add(Entry);
}

