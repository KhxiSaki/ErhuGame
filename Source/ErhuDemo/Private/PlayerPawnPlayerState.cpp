// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnPlayerState.h"
#include "AbilitySystemComponent.h"
APlayerPawnPlayerState::APlayerPawnPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	SetNetUpdateFrequency(100.0f);

}

UAbilitySystemComponent * APlayerPawnPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}