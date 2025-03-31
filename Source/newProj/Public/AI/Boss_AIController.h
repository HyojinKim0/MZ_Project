// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss_AIController.generated.h"

/**
 * 
 */
UCLASS()
class NEWPROJ_API ABoss_AIController : public AAIController
{
	GENERATED_BODY()

public:
	//»ý¼ºÀÚ
	ABoss_AIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn)override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	UPROPERTY()
	class UBehaviorTree* BTAsset;

};
