// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AI/DefineAI.h"
#include "AI/Boss_AIController.h"
#include "AI/BossAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//AI로 부터 제어하고 있는 폰의 정보를 가져온다 
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}
	
	//폰의 위치와 폰이속한 월드의 값을 가져온다 
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;

	}

	//우리가 구현할 AI인터페이스 형변환해서 가져옴 
	IBossAIInterface* AIPawn = Cast<IBossAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	//인터페이스로부터 감지할 영역(반경) 가져옴 
	float DetectRadius = AIPawn->GetAIDetectRange();

	//오버랩함수를 사용해서 체크 -> 오버랩된 결과가 배열안에 들어오게 된다 
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	//감지가 된다면 모든폰에대해서 하나씩 검사 수행 
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			//폰을 조종하는게 플레이어 컨트롤러인 경우 -> 플레이어 
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
			
		}
	}
	//플레이어를 못찾거나 찾지 못했을경우 null값 설정, 붉은색 
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}
