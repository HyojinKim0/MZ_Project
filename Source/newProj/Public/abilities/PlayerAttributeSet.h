// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class NEWPROJ_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

public:
	//체력
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData health;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, health); //getter,setter 메크로

	//기력
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData stamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, stamina); //getter,setter 메크로

	//공격 사정거리
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData attackRange;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, attackRange); //getter,setter 메크로

	//쿨타임
	UPROPERTY(BlueprintReadOnly, Category = "Cooldown")
	FGameplayAttributeData Cooldown;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Cooldown) //getter,setter 메크로

	
};
