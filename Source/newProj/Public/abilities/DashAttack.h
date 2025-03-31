// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DashAttack.generated.h"

/**
 * 
 */
UCLASS()
class NEWPROJ_API UDashAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDashAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
public:
	//쿨타임 태그 저장 변수
	FGameplayTagContainer CooldownTags;

	UPROPERTY(EditDefaultsOnly, Category = "Montag")
	UAnimMontage* dashAttack;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	TSubclassOf<UGameplayEffect> coolDown;

};
