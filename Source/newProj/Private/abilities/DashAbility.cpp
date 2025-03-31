// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/DashAbility.h"
#include "GameFramework/Character.h"

UDashAbility::UDashAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo->AvatarActor.IsValid())
	{
		ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

		if (character)
		{
			FVector DashDirection = character->GetActorForwardVector() * 1000.f;
			character->LaunchCharacter(DashDirection, true, false);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
