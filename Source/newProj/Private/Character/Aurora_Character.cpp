// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Aurora_Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//입력관련
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>
//박스 콜리젼
#include "Components/BoxComponent.h"

#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/KismetMathLibrary.h"
//어빌리티 어트리뷰트
#include "abilities/PlayerAttributeSet.h"
//어빌리티 컴포넌트
#include "AbilitySystemComponent.h"

#include "Character/Character_StatComponent.h"
//어빌리티 대쉬
#include "abilities/DashAbility.h"
#include "abilities/DashAttack.h"

AAurora_Character::AAurora_Character()
{

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/GlacialEmpress/Meshes/Aurora_GlacialEmpress.Aurora_GlacialEmpress'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//스프링암 
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 250;
	springArmComp->bUsePawnControlRotation = true;

	//카메라
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = true;

	FName WeaponSocket(TEXT("Sword_Base"));

	Weapon = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon"));
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	/*Weapon->SetupAttachment(GetMesh(), WeaponSocket);*/
	Weapon->SetRelativeLocation(FVector(50, 0, 0));
	Weapon->SetRelativeScale3D(FVector(1.7f, .25f, .25f));
	Weapon->SetCollisionProfileName(TEXT("Custom"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//어빌리티 시스템
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AAurora_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAurora_Character::BeginPlay()
{
	Super::BeginPlay();

	if (Weapon)
	{
		//공격 사거리
		FVector startPos = GetMesh()->GetSocketLocation("Sword_Base");
		FVector endPos = GetMesh()->GetSocketLocation("Sword_Tip");

		auto atkrange = UKismetMathLibrary::VSize(startPos - endPos);

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%f"), atkrange));

		statComp->SetAtkRange(atkrange);

		//오버랩
		Weapon->OnComponentBeginOverlap.AddDynamic(this, &AAurora_Character::OnOverlapBegin);
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (DashAttack)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DashAttack, 1, 0));
		}
	}

	GiveAbilities();
}

void AAurora_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput)
	{
		PlayerInput->BindAction(ia_DashAttack, ETriggerEvent::Started, this, &AAurora_Character::ActivateDashAttack);
	}
}

void AAurora_Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("enemy"))) //액터 태그 이름이 enemy이면
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Hit")));

		FVector startPos = GetMesh()->GetSocketLocation("Sword_Base"); //오버랩 시작지점
		FVector endPos = GetMesh()->GetSocketLocation("Sword_Tip"); //오버랩 종료지점
		TArray<AActor*> ignoretoActor; //오버랩 무시할 액터 배열
		FHitResult _hit; //맞음 결과
		FCollisionQueryParams params;

		params.bTraceComplex = true;
		params.AddIgnoredActor(this);
		params.AddIgnoredComponent(Weapon);

		bool bHit = UKismetSystemLibrary::SphereTraceSingle(this, startPos, endPos, 10, ETraceTypeQuery::TraceTypeQuery3,
			false, ignoretoActor, EDrawDebugTrace::ForDuration, _hit, true);

		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("bHit")));
		}

	}
}

void AAurora_Character::Sprint()
{
	if (isAttack || saveAttack)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	isSprint = true;

}

void AAurora_Character::Attack()
{
	if (isAttack == true)
	{
		if (GetCharacterMovement())
		{
			if (!GetCharacterMovement()->IsFalling() && !isSprint)
			{
				saveAttack = true;
				GetCharacterMovement()->MaxWalkSpeed = 0.f;
				GetCharacterMovement()->bOrientRotationToMovement = false;
				JumpMaxCount = 0;
			}
		}
	}
	else
	{
		if (GetCharacterMovement())
		{
			if (!GetCharacterMovement()->IsFalling() && !isSprint)
			{
				isAttack = true;
				GetCharacterMovement()->MaxWalkSpeed = 0.f;
				GetCharacterMovement()->bOrientRotationToMovement = false;
				JumpMaxCount = 0;
				ComboAttack();
			}
		}
	}
}

void AAurora_Character::ComboAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	switch (attackCount)
	{
	case 0:
		attackCount++;
		AnimInstance->Montage_Play(Attak1);
		AnimInstance->Montage_SetPlayRate(Attak1, 1.f);

		break;
	case 1:
		attackCount++;
		AnimInstance->Montage_Play(Attak2);
		AnimInstance->Montage_SetPlayRate(Attak2, 1.f);
		break;
	case 2:
		attackCount = 0;
		AnimInstance->Montage_Play(Attak3);
		AnimInstance->Montage_SetPlayRate(Attak3, 1.f);
		break;
	default:
		break;
	}
}

void AAurora_Character::ResetCombo()
{
	attackCount = 0;
	saveAttack = false;
	isAttack = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		JumpMaxCount = 1;
	}
}

void AAurora_Character::GiveAbilities() 
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec DashAttackSpec(DashAttack, 1);
		AbilitySystemComponent->GiveAbility(DashAttackSpec); //스킬 등록
	}
}

void AAurora_Character::ActivateDashAttack()
{
	if (AbilitySystemComponent && DashAttack && !GetCharacterMovement()->IsFalling())
	{
		JumpMaxCount = 0;
		AbilitySystemComponent->TryActivateAbilityByClass(DashAttack);
	}
}
