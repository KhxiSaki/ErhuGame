// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "AbilitySystemComponent.h"
#include "PlayerPawnPlayerState.h"

// Sets default values
APlayerPawn::APlayerPawn(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent * APlayerPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void APlayerPawn::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	APlayerPawnPlayerState* PS = GetPlayerState<APlayerPawnPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

void APlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	APlayerPawnPlayerState* PS = GetPlayerState<APlayerPawnPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
}
// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

