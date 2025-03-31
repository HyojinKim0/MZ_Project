// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Boss.generated.h"
 // 보스 상태 열거형
UENUM(BlueprintType)
enum class EBossState : uint8
{
	Intro,
	Idle,
	Patrolling,
	Chasing,
	Attack,
	Damaged,
	Damaged_Back,
	Groggy,
	Dying
};
UCLASS()
class NEWPROJ_API ABase_Boss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	//보스체력 700
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	float MaxHealth;
	// 현재체력 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Stats")
	float CurrentHealth;
	//이동속도 
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Boss Stats")
	float MovementSpeed;

	//데미지 때 호출되는 함수 
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void TakeDamage(float DamageAmount);

	// 보스 사망
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();
	// 보스 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Stats")
	EBossState CurrentState;

public:
	// 현재 체력Get
	UFUNCTION(BlueprintPure, Category = "Boss Stats")
	float GetCurrentHealth() const;

	// 최대 체력 Get
	UFUNCTION(BlueprintPure, Category = "Boss Stats")
	float GetMaxHealth() const;

	// 현재 상태 Get
	UFUNCTION(BlueprintPure, Category = "Boss Stats")
	EBossState GetCurrentState() const;

	
};
