// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Base_Boss.h"
#include "Engine/StreamableManager.h"
#include "AI/BossAIInterface.h"
#include "Rampage_Boss.generated.h"


/**
 * 
 */
UCLASS()
class NEWPROJ_API ARampage_Boss : public ABase_Boss , public IBossAIInterface
{
	GENERATED_BODY()
	
public:
    // 생성자
    ARampage_Boss();

protected:
    
    virtual void BeginPlay() override;

    // 스켈레탈메시 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    USkeletalMeshComponent* BossMeshComponent;

    //AttackHitBox_L , AttackHItBox_R
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class UBoxComponent* AttackHitBox_L;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class UBoxComponent* AttackHitBox_R;

    //백어택 판정 박스 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class UBoxComponent* Back_Damaged_HitBox;

protected:
    virtual float GetAIPatrolRadius()override;
    virtual float GetAIDetectRange()override;
    virtual float GetAIAttackRange()override;
    virtual float GetAITurnSpeed()override;

public:
    // 메시 컴포넌트 Get
    UFUNCTION(BlueprintPure, Category = "Mesh")
    USkeletalMeshComponent* GetBossMeshComponent() const;
};
