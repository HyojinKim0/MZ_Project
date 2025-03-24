// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/FSM_Boss.h"

// Sets default values for this component's properties
UFSM_Boss::UFSM_Boss()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFSM_Boss::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFSM_Boss::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

