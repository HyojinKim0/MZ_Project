// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/DashAttackEffect.h"
#include "abilities/PlayerAttributeSet.h"
#include "GameplayTagContainer.h"

UDashAttackEffect::UDashAttackEffect()
{
    //지속시간
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
    DurationMagnitude = FScalableFloat(5.0f);

    // 태그 설정
    InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Skill"))); //왠지 모르게 설정이 안됨/ 블루프린트에서 따로 설정
    UE_LOG(LogTemp, Warning, TEXT("Cooldown.Skill 태그가 추가되었습니다!"));

    // 쿨타임 설정 (모디파이 설정)
    FGameplayModifierInfo Modifier;
    Modifier.Attribute = UPlayerAttributeSet::GetCooldownAttribute();//어트리뷰트의 쿨타임 참조
    Modifier.ModifierOp = EGameplayModOp::Override;
    Modifier.ModifierMagnitude = FScalableFloat(5.0f);

    Modifiers.Add(Modifier);
}
