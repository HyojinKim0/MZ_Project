// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Character.generated.h"

struct FInputActionValue;
class UCharacter_StatComponent;
class UEnhancedInputComponent;
class UInputMappingContext;
class UInputAction;
class UMotionWarpingComponent;

UCLASS()
class NEWPROJ_API ABase_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	UCharacter_StatComponent* statComp;

protected:
	//입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* imc_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Sprint;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Lockon;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Jump;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Montag")
	UAnimMontage* avoid;

protected:
	//바라볼 타겟 지정 추후 보스 class로 바꾸거나 아님 그대로 둘 예정
	UPROPERTY()
	AActor* LockedOnTarget;

public:
	UPROPERTY()
	UMotionWarpingComponent* MotionWarpingComponent;

protected:
	//이동 로직
	void Input_Move(const FInputActionValue& InputActionValue);
	void PlayerMove();
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Turn(const struct FInputActionValue& inputValue);

	//시점 고정
	void CameraLock();
	//쉬프트 눌렀을때
	void OnShift();

	//공격, 달리기
	virtual void Attack() {};
	void Avoid();
	void ResetAvoid();
	virtual void Sprint() {};
	void NoSprint();

	void GetDamage(float damage);

public:
	FVector dir;

	FRotator NewRotation = FRotator(0.f, 0.f, 0.f); //보스 바라볼 방향을 저장하는 변수

	FTimerHandle DodgeTimerHandle; //타이머 핸들

	float LastShiftPressTime = 0.f; //마지막으로 쉬프트 누른 시간
	float DoubleTap = 0.3f; //더블탭 허용 시간

	bool isAvoid = false; // 구르기 중인가

	bool isAttack = false; //공격 중인가
	bool saveAttack = false; //콤보 공격을 할것인가
	int32 attackCount = 0; //콤보 공격횟수

	bool isSprint = false; //달리기 상태인가
};
