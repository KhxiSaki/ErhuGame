// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SmartObjectRuntime.h"
#include "AbilityTask_UseSmartObject.generated.h"

/*class AAIController;*/
/*class UAITask_MoveTo;*/
class APlayerController;
class UGameplayBehavior;
class USmartObjectComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUseSmartObjectDelegate);

/**
 * 
 */
UCLASS()
class ERHUDEMO_API UAbilityTask_UseSmartObject : public UAbilityTask
{
	GENERATED_BODY()
public:
UAbilityTask_UseSmartObject(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(BlueprintAssignable)
	FUseSmartObjectDelegate	OnSucceeded;

	UPROPERTY(BlueprintAssignable)
	FUseSmartObjectDelegate	OnFailed;

	/**
	 * Helper function to create an AITask that interacts with the SmartObject slot using the GameplayBehavior definition
	 * This version starts the interaction on spot so the actor needs to be at the desired position.
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param ClaimHandle The handle to an already claimed slot.
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="UseSmartObjectWithGameplayBehavior",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static  UAbilityTask_UseSmartObject* UseSmartObjectWithGameplayBehavior(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, FSmartObjectClaimHandle ClaimHandle);

	virtual void Activate() override;

	 virtual FString GetDebugString() const override;
	 bool StartInteraction();

	 void Abort();

	 void OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted);

	 void OnSlotInvalidated(const FSmartObjectClaimHandle& ClaimHandle, const ESmartObjectSlotState State);

protected:

	 virtual void OnDestroy(bool AbilityEnded) override;
	
	FDelegateHandle OnSuccessfulHandle;
	FDelegateHandle OnFailedHandle;

		UPROPERTY()
    	TObjectPtr<UGameplayBehavior> GameplayBehavior;
    
    	FSmartObjectClaimHandle ClaimedHandle;
    	FDelegateHandle OnBehaviorFinishedNotifyHandle;
    
    	bool bBehaviorFinished;
};
