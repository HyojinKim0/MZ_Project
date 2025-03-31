// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/DashAttack.h"
#include "GameFramework/Character.h"
#include "abilities/PlayerAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "abilities/DashAttackEffect.h"

UDashAttack::UDashAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 쿨타임 적용할 GameplayEffect 클래스 설정
	CooldownGameplayEffectClass = UDashAttackEffect::StaticClass();
	// 쿨타임 태그 설정
	CooldownTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Skill")));

	// 태그 추가
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.DashAttack")));
}

void UDashAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) // 코스트 및 쿨타임 적용
	{
		return;
	}

	ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!character)
		return;

	if (dashAttack)
	{
		character->GetMesh()->GetAnimInstance()->Montage_Play(dashAttack);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}