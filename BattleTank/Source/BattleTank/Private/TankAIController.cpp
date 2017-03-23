// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "Tank.h"  // So we can implement OnDeath
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

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossedTankDeath);
	}
}

void ATankAIController::OnPossedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Recived by AI Controller!!"));
	if (!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}
