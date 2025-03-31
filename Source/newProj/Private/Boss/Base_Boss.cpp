// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Base_Boss.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ABase_Boss::ABase_Boss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//체력 세팅 
	MaxHealth = 700.0f;
	CurrentHealth = MaxHealth;
	
	// 이동속도 세팅 
	if (GetCharacterMovement())
	{
		
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
		GetCharacterMovement()->AirControl = 0.2f;
	}
	// 기본 이동 속도 설정
	MovementSpeed = 480.0f;

	//상태 설정 
	CurrentState = EBossState::Intro;
}

// Called when the game starts or when spawned
void ABase_Boss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase_Boss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABase_Boss::TakeDamage(float DamageAmount)
{

	// 현재 체력에서 데미지만큼 감소
	CurrentHealth -= DamageAmount;

	// 체력이 0 이하로 떨어지면 사망 
	if (CurrentHealth <= 0.0f)
	{
		Die();
	}
}

void ABase_Boss::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Boss has died!"));

	// 액터 제거
	Destroy();
}

float ABase_Boss::GetCurrentHealth() const
{
	return CurrentHealth;
}

float ABase_Boss::GetMaxHealth() const
{
	return  MaxHealth;
}

EBossState ABase_Boss::GetCurrentState() const
{
	return CurrentState;
}

