// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/PlayerAttributeSet.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	//기본 쿨타임
	Cooldown.SetBaseValue(0.0f);
	//현재 쿨타임
	Cooldown.SetCurrentValue(0.0f);

	//기본체력
	health.SetBaseValue(120.f);
	//현재체력
	health.SetCurrentValue(120.f);

	//기본스테미나
	stamina.SetBaseValue(100.f);
	//현재스테미나
	stamina.SetCurrentValue(100.f);
}
