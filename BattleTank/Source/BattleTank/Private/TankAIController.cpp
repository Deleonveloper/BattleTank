// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);
	// Aim towards the player
	auto AiminComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AiminComponent->AimAt(PlayerTank->GetActorLocation());

	if(AiminComponent->GetFiringState() == EFiringState::Looked)
	{
		AiminComponent->Fire(); // TODO limit firing rate
	}
	
}
