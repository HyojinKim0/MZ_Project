// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Rampage_Boss.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "AI/Boss_AIController.h"


ARampage_Boss::ARampage_Boss()
{
	//생성자
	//AIController 
	AIControllerClass = ABoss_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//캡슐 콜라이더 조정 
	GetCapsuleComponent()->SetCapsuleHalfHeight(350.0f);  // 높이 조정
	GetCapsuleComponent()->SetCapsuleRadius(140.0f);       // 반지름 조정
	GetCapsuleComponent()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f,0.0f), FRotator(0.0f, 0.0f, 0.0f));//캡슐 상대위치 조정

	
	
	//양팔 소켓이름
	FName L_AttackHitBox_Socket(TEXT("Muzzle_02"));
	FName R_AttackHitBox_Socket(TEXT("Muzzle_01"));
	FName Back_DamagedBox_Socket(TEXT("Back_Damaged01"));

	//AttackHitBox_L , AttackHItBox_L 조정 
	AttackHitBox_L = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox_L"));
	AttackHitBox_L->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, L_AttackHitBox_Socket);
	AttackHitBox_L->SetRelativeLocation(FVector(50, 0,0));
	AttackHitBox_L->SetRelativeScale3D(FVector(1.9f, .7f, .8f));
	AttackHitBox_L->SetCollisionProfileName(TEXT("Custom"));
	AttackHitBox_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	

	//AttackHitBox_R ,AttackHitBox_R조정 
	AttackHitBox_R = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox_R"));
	AttackHitBox_R->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, R_AttackHitBox_Socket);
	AttackHitBox_R->SetRelativeLocation(FVector(-45, 0, 0));
	AttackHitBox_R->SetRelativeScale3D(FVector(1.9f, .7f, .8f));
	AttackHitBox_R->SetCollisionProfileName(TEXT("Custom"));
	AttackHitBox_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*
	//백어택 판정 박스 조정  
	//Damaged_Back_HitBox
	Back_Damaged_HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damaged_Back_HitBox"));
	Back_Damaged_HitBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Back_DamagedBox_Socket);
	Back_Damaged_HitBox->SetRelativeLocation(FVector(-45, 0, 0));
	Back_Damaged_HitBox->SetRelativeScale3D(FVector(1.9f, .7f, .8f));
	Back_Damaged_HitBox->SetCollisionProfileName(TEXT("Custom"));
	Back_Damaged_HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	*/
	//Mesh 조정 
	GetMesh()->SetRelativeLocationAndRotation(FVector(-20.0f, 0.0f, -340.0f), FRotator(0.0f, -90.0f, 0.0f));// 메쉬 상대위치 조정 
	GetMesh()->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	BossMeshComponent = Cast<USkeletalMeshComponent>(GetMesh());


	//메쉬 참조 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	


}

void ARampage_Boss::BeginPlay()
{
	//부모 클래스꺼 호출 
	Super::BeginPlay();
}

float ARampage_Boss::GetAIPatrolRadius()
{
	return 1000.0f;
}

float ARampage_Boss::GetAIDetectRange()
{
	return 600.0f;
}

float ARampage_Boss::GetAIAttackRange()
{
	// 스탯 에서 Attack Radius 값을 가져와서 구성해야함 
	return 0.0f;
}

float ARampage_Boss::GetAITurnSpeed()
{
	return 0.0f;
}

USkeletalMeshComponent* ARampage_Boss::GetBossMeshComponent() const
{
	return BossMeshComponent;
}
