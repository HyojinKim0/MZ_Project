// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character_StatComponent.generated.h"

class UPlayerAttributeSet;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWPROJ_API UCharacter_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacter_StatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UPlayerAttributeSet* AttributeSet;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float currentStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float maxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AtkRange")
	float atkRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool isDead = false;

public:
	void DecreaseHealth(float amount);
	void IncreaseHealth(float amount);


	void DecreaseStamina(float amount);
	void IncreaseStamina(float amount);

	void SetAtkRange(float amount);

};
