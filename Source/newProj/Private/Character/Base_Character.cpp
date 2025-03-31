// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base_Character.h"

//스텟 컴포넌트
#include "Character/Character_StatComponent.h"

//입력관련
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>
//캐릭터 이동 컴포넌트
#include "GameFramework/CharacterMovementComponent.h"
//Kismet
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
//모션워핑
#include "MotionWarpingComponent.h"

// Sets default values
ABase_Character::ABase_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	//스텟 컴포넌트
	statComp = CreateDefaultSubobject<UCharacter_StatComponent>(TEXT("PlayerStat"));
}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();
	
	auto pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsystem)
		{
			subsystem->AddMappingContext(imc_Player, 0);
		}
	}
}

// Called every frame
void ABase_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();

	if (LockedOnTarget) //락온된 대상이 있으면 실행
	{
		FVector CameraLocation = this->GetActorLocation();
		FVector TargetLocation = LockedOnTarget->GetActorLocation() + FVector(0.f, 0.f, -100.f); // 타겟의 위치를 보도록 조정

		// 타겟을 향한 회전 계산
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);

		// 카메라가 부드럽게 회전하도록 보정
		NewRotation = FMath::RInterpTo(
			this->GetControlRotation(),
			LookAtRotation,
			DeltaTime,
			5.0f // 회전 속도 (값이 클수록 빠르게 타겟을 바라봄)
		);

		this->GetController()->SetControlRotation(NewRotation);
	}
	else
	{
		// LockedOnTarget이 없을 경우 일반적인 회전 방식
		FRotator ControlRotation = GetControlRotation();
		ControlRotation.Pitch = 0.f; // 카메라 회전이 아닌, 캐릭터만 회전하도록 처리

		// 캐릭터 회전 적용
		this->SetActorRotation(ControlRotation);
	}

	if (!isAttack && !isSprint && !isAvoid) //공격, 달리기, 회피 상태가 아닐때 틱당 스태미나 3회복
	{
		statComp->IncreaseStamina(3);
	}

	/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%f"), statComp->currentStamina));*/

}

// Called to bind functionality to input
void ABase_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput)
	{
		PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &ABase_Character::Input_Move);
		PlayerInput->BindAction(ia_Look, ETriggerEvent::Triggered, this, &ABase_Character::Input_Look);
		PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &ABase_Character::Input_Turn);
		PlayerInput->BindAction(ia_Attack, ETriggerEvent::Started, this, &ABase_Character::Attack);
		PlayerInput->BindAction(ia_Sprint, ETriggerEvent::Started, this, &ABase_Character::OnShift);
		PlayerInput->BindAction(ia_Sprint, ETriggerEvent::Completed, this, &ABase_Character::NoSprint);
		PlayerInput->BindAction(ia_Lockon, ETriggerEvent::Started, this, &ABase_Character::CameraLock);
		PlayerInput->BindAction(ia_Jump, ETriggerEvent::Started, this, &ABase_Character::Jump);
		PlayerInput->BindAction(ia_Jump, ETriggerEvent::Completed, this, &ABase_Character::StopJumping);
	}
	
}

void ABase_Character::Input_Move(const FInputActionValue& InputActionValue)
{
	FVector2D value = InputActionValue.Get<FVector2D>();

	dir.X = value.X;
	dir.Y = value.Y;
}

void ABase_Character::PlayerMove()
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	// 방향 변환
	dir = FTransform(ControlRotation).TransformVector(dir);

	// 이동 실행
	AddMovementInput(dir);

	// 방향 벡터 초기화
	dir = FVector::ZeroVector;
}

void ABase_Character::Input_Look(const FInputActionValue& InputActionValue)
{
	if (LockedOnTarget)
	{
		return;
	}
	float value = InputActionValue.Get<float>();
	AddControllerPitchInput(value);
}

void ABase_Character::Input_Turn(const FInputActionValue& inputValue)
{
	if (LockedOnTarget)
	{
		return;
	}
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void ABase_Character::OnShift()
{
	float currentTime = GetWorld()->GetTimeSeconds();
	float lastShift = currentTime - LastShiftPressTime;

	if (lastShift <= DoubleTap)
	{
		Avoid();
	}
	else
	{
		Sprint();
	}

	LastShiftPressTime = currentTime;
}

void ABase_Character::Avoid()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!isAvoid)
	{
		if (statComp->currentStamina <= 0 || statComp->currentStamina - 22.f <= 0) //현재 스테미너가 0이거나 실행했을 경우 음수인 경우 리턴
		{
			return;
		}

		statComp->DecreaseStamina(22.f);

		this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

		AnimInstance->Montage_Play(avoid);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Avoid")));

		//모션 워핑
		FMotionWarpingTarget Target = {};
		Target.Name = FName("D_warpStart"); //모션워핑 이름
		Target.Location = GetActorLocation();
		Target.Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), this->GetActorLocation()); //바라보는 회전값

		MotionWarpingComponent->AddOrUpdateWarpTarget(Target);

		isAvoid = true;
	}
	else
	{
		return;
	}

	GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &ABase_Character::ResetAvoid, 1.0f, false);
}

void ABase_Character::ResetAvoid()
{
	isAvoid = false;

	this->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ABase_Character::NoSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	isSprint = false;
}

void ABase_Character::GetDamage(float damage)
{
	statComp->DecreaseHealth(damage);
}

void ABase_Character::CameraLock()
{
	if (LockedOnTarget != nullptr) //이미 락온 중이면 해제
	{
		LockedOnTarget = nullptr;
		return;
	}

	float SearchRadius = 500.f;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector PlayerLocation = GetActorLocation();

	// 탐색할 액터를 저장할 배열
	TArray<AActor*> OverlappingActors;

	// 충돌 필터 설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); //Enemy

	// 검색 제외 대상 설정 (자기 자신 제외)
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// 액터 탐색
	bool bHasHit = UKismetSystemLibrary::SphereOverlapActors(
		World,                         // 현재 월드
		PlayerLocation,                 // 탐색 중심 위치
		SearchRadius,                   // 탐색 반경
		ObjectTypes,                     // 검색할 오브젝트 타입
		AActor::StaticClass(),           // 찾을 액터의 기본 클래스 (모든 액터)
		ActorsToIgnore,                   // 제외할 액터 리스트
		OverlappingActors                 // 결과 저장 배열
	);

	// 디버그: 탐색 반경을 시각적으로 표시
	DrawDebugSphere(World, PlayerLocation, SearchRadius, 16, FColor::Red, false, 2.0f);

	// 결과 출력
	if (bHasHit)
	{
		for (AActor* NearbyActor : OverlappingActors)
		{
			if (NearbyActor->ActorHasTag(FName("enemy"))) //엑터 태그가 enemy일 경우
			{
				LockedOnTarget = NearbyActor;
				UE_LOG(LogTemp, Warning, TEXT("Locked onto: %s"), *NearbyActor->GetName());
			}
		}
	}
	else
	{
		LockedOnTarget = nullptr;
	}
}
