// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "PlayerPawn.generated.h"

UCLASS()
class ERHUDEMO_API APlayerPawn : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPawn(const class FObjectInitializer& ObjectInitializer);

// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	// Client only
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
