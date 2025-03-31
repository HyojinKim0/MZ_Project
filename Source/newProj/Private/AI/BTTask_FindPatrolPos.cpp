// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "AI/DefineAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BossAIInterface.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{


}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//다음 정찰위치(정찰기능)를 찾는 태스크 
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//비헤이비어 트리를 소유한 컴포넌트의 오너 가져온다 
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;

	}
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}
	//interface의 함수를 통해서 값을 받아오기 원하기 때문에 캐스팅되는지 확인 작업 
	IBossAIInterface* AIPawn = Cast<IBossAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;

	}


	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float PatrolRadius = AIPawn->GetAIPatrolRadius();

	FNavLocation NextPatrolPos;

	//범위 
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROL_POS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
