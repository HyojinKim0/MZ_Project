// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_StatComponent.h"
//#include "abilities/PlayerAttributeSet.h"

UCharacter_StatComponent::UCharacter_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	maxHealth = 120.f;
	currentHealth = maxHealth;

	maxStamina = 100.f;
	currentStamina = maxStamina;
}


// Called when the game starts
void UCharacter_StatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UCharacter_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacter_StatComponent::DecreaseHealth(float amount)
{
	currentHealth -= amount;
}

void UCharacter_StatComponent::IncreaseHealth(float amount)
{
	currentHealth += amount;

	if (currentHealth >= maxHealth)
	{
		currentHealth = maxHealth;
	}
}

void UCharacter_StatComponent::DecreaseStamina(float amount)
{
	currentStamina -= amount;
	//AttributeSet->stamina.SetCurrentValue(currentStamina);
}

void UCharacter_StatComponent::IncreaseStamina(float amount)
{
	currentStamina += amount;

	if (currentStamina >= maxStamina)
	{
		currentStamina = maxStamina;
	}
}

void UCharacter_StatComponent::SetAtkRange(float amount)
{
	atkRange = amount;
}

