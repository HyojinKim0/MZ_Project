// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base_Character.h"
#include "AbilitySystemInterface.h"
#include "Aurora_Character.generated.h"

class AbilitySystemComponent;
class UPlayerAttributeSet;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UGameplayAbility;

UCLASS()
class NEWPROJ_API AAurora_Character : public ABase_Character, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAurora_Character();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UPlayerAttributeSet* AttributeSet;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* tpsCamComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* Weapon;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Montag")
	UAnimMontage* Attak1;
	UPROPERTY(EditDefaultsOnly, Category = "Montag")
	UAnimMontage* Attak2;
	UPROPERTY(EditDefaultsOnly, Category = "Montag")
	UAnimMontage* Attak3;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_DashAttack;

private:
	virtual void Sprint() override;

public:
	virtual void Attack() override;

public:
	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	UFUNCTION(BlueprintCallable)
	void ResetCombo();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> DashAttack;
private:
	//능력 추가 함수
	void GiveAbilities();
	//능력 실행
	void ActivateDashAttack();

public:
	float Stamina = 0.f;
};
