// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_UseSmartObject.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameplayBehavior.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorSubsystem.h"
#include "GameplayTagAssetInterface.h"
#include "Misc/ScopeExit.h"
#include "SmartObjectComponent.h"
#include "SmartObjectRequestTypes.h"
#include "SmartObjectSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityTask_UseSmartObject)

UAbilityTask_UseSmartObject::UAbilityTask_UseSmartObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bBehaviorFinished = false;
}

UAbilityTask_UseSmartObject* UAbilityTask_UseSmartObject::UseSmartObjectWithGameplayBehavior(
	UGameplayAbility* OwningAbility, FName TaskInstanceName,
	FSmartObjectClaimHandle ClaimHandle)
{
	
	UAbilityTask_UseSmartObject* MyObj = NewAbilityTask<UAbilityTask_UseSmartObject>(OwningAbility, TaskInstanceName);
	MyObj->ClaimedHandle = ClaimHandle;
	return MyObj;
}

void UAbilityTask_UseSmartObject::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(GetAvatarActor()->GetWorld());
	if (!ensureMsgf(SmartObjectSubsystem != nullptr, TEXT("SmartObjectSubsystem must be accessible at this point.")))
	{
		return;
	}

	// A valid claimed handle can point to an object that is no longer part of the simulation
	if (!SmartObjectSubsystem || !SmartObjectSubsystem->IsClaimedSmartObjectValid(ClaimedHandle))
	{
		UE_VLOG(GetAvatarActor(), LogSmartObject, Log, TEXT("Claim handle: %s refers to an object that is no longer available."), *LexToString(ClaimedHandle));
		return;
	}

	// Register a callback to be notified if the claimed slot became unavailable
	SmartObjectSubsystem->RegisterSlotInvalidationCallback(ClaimedHandle, FOnSlotInvalidated::CreateUObject(this, &UAbilityTask_UseSmartObject::OnSlotInvalidated));
    if (!StartInteraction())
	{
		OnFailed.Broadcast();
	}
	
}

bool UAbilityTask_UseSmartObject::StartInteraction()
{
	
	UWorld* World = GetAvatarActor()->GetWorld();
	USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(World);
	if (!ensure(SmartObjectSubsystem))
	{
		return false;
	}

	const UGameplayBehaviorSmartObjectBehaviorDefinition* SmartObjectGameplayBehaviorDefinition = SmartObjectSubsystem->MarkSlotAsOccupied<UGameplayBehaviorSmartObjectBehaviorDefinition>(ClaimedHandle);
	const UGameplayBehaviorConfig* GameplayBehaviorConfig = SmartObjectGameplayBehaviorDefinition != nullptr ? SmartObjectGameplayBehaviorDefinition->GameplayBehaviorConfig : nullptr;
	GameplayBehavior = GameplayBehaviorConfig != nullptr ? GameplayBehaviorConfig->GetBehavior(*World) : nullptr;
	if (GameplayBehavior == nullptr)
	{
		return false;
	}

	const USmartObjectComponent* SmartObjectComponent = SmartObjectSubsystem->GetSmartObjectComponent(ClaimedHandle);
	AActor& InteractorActor = *GetAvatarActor();
	AActor* InteracteeActor = SmartObjectComponent ? SmartObjectComponent->GetOwner() : nullptr;
	const bool bBehaviorActive = UGameplayBehaviorSubsystem::TriggerBehavior(*GameplayBehavior, InteractorActor, GameplayBehaviorConfig, InteracteeActor);
	// Behavior can be successfully triggered AND ended synchronously. We are only interested to register callback when still running
	if (bBehaviorActive)
	{
		OnBehaviorFinishedNotifyHandle = GameplayBehavior->GetOnBehaviorFinishedDelegate().AddUObject(this, &UAbilityTask_UseSmartObject::OnSmartObjectBehaviorFinished);
	}

	return bBehaviorActive;
}

void UAbilityTask_UseSmartObject::OnDestroy(bool AbilityEnded)
{
if (ClaimedHandle.IsValid())
	{
		USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(GetAvatarActor()->GetWorld());
		check(SmartObjectSubsystem);
		SmartObjectSubsystem->MarkSlotAsFree(ClaimedHandle);
		SmartObjectSubsystem->UnregisterSlotInvalidationCallback(ClaimedHandle);
		ClaimedHandle.Invalidate();
	}
	

	Super::OnDestroy(AbilityEnded);

}

void UAbilityTask_UseSmartObject::OnSlotInvalidated(const FSmartObjectClaimHandle& ClaimHandle, ESmartObjectSlotState State)
{
	Abort();
}

void UAbilityTask_UseSmartObject::Abort()
{
	if (GameplayBehavior != nullptr)
	{
		GameplayBehavior->GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		GameplayBehavior->AbortBehavior(*GetAvatarActor());
		OnFailed.Broadcast();
	}
}

void UAbilityTask_UseSmartObject::OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted)
{

	// make sure we handle the right pawn - we can get this notify for a different
	// Avatar if the behavior sending it out is not instanced (CDO is being used to perform actions)
	if (GetAvatarActor() == &Avatar)
	{
		Behavior.GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		OnSucceeded.Broadcast();
	}
}

FString UAbilityTask_UseSmartObject::GetDebugString() const
{

	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		
	}

	return FString::Printf(TEXT("PlayMontageAndWait. MontageToPlay:   (Currently Playing):"));
}
