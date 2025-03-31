// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Aurora_AnimInstance.generated.h"

UCLASS()
class NEWPROJ_API UAurora_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float direction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool isInAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool isAccelerating = false;

private:
	UFUNCTION()
	void AnimNotify_SaveAttack();
	UFUNCTION()
	void AnimNotify_ResetCombo();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_AttackStart();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_DodgeStart();
	UFUNCTION()
	void AnimNotify_DodgeEnd();
	
};
