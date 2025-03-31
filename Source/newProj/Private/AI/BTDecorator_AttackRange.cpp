// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackRange.h"
#include "AI/DefineAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BossAIInterface.h"

UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//  상위클래스의 값을 받아온뒤 
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	//Task_FindPatrol 과 비슷하게 전개 시킨다 

	//폰가져오기 
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	//폰으로 행동 정의 인터페이스 가져오기 
	IBossAIInterface* AIPawn = Cast<IBossAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}
	
	//블랙보드에서 Target 값 가져오기 
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}
	//가져온 타겟값으로 거리구하기 
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	//AI가 판단한 거리내에 들어오면 
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);

	//True 리턴 
	return bResult;
}
