// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Aurora_AnimInstance.h"

#include "Character/Aurora_Character.h"
//movemet
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
//¹Ú½º ÄÝ¸®Á¯
#include "Components/BoxComponent.h"
//Ä¸½¶ ÄÝ¸®Àü
#include "Components/CapsuleComponent.h"

void UAurora_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<AAurora_Character>(ownerPawn);

	if (player)
	{
		FVector velocity = player->GetVelocity();

		FVector forwardVector = player->GetActorForwardVector();

		speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
		
		if (UKismetMathLibrary::VSize(player->GetCharacterMovement()->GetCurrentAcceleration()) > 0)
		{
			isAccelerating = true;
		}
		else
		{
			isAccelerating = false;
		}
	}
}

void UAurora_AnimInstance::AnimNotify_SaveAttack()
{
	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<AAurora_Character>(ownerPawn);
	if (player->saveAttack == true)
	{
		player->ComboAttack();
		player->saveAttack = false;
	}
}

void UAurora_AnimInstance::AnimNotify_ResetCombo()
{
	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<AAurora_Character>(ownerPawn);

	player->ResetCombo();
}

void UAurora_AnimInstance::AnimNotify_AttackStart()
{
	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<AAurora_Character>(ownerPawn);

	player->Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UAurora_AnimInstance::AnimNotify_AttackEnd()
{
	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<AAurora_Character>(ownerPawn);

	player->Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UAurora_AnimInstance::AnimNotify_DodgeStart()
{
	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<AAurora_Character>(ownerPawn);
	player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UAurora_AnimInstance::AnimNotify_DodgeEnd()
{
	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<AAurora_Character>(ownerPawn);
	player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
