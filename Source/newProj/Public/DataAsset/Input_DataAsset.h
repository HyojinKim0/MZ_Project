// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Input_DataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FCharacterInput
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* inputAction; //InputAction 파일 받는 곳

	bool IsValid() const
	{
		return inputAction != nullptr;
	}
};

UCLASS()
class NEWPROJ_API UInput_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext; //mapping context 파일 받는 곳

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	//FWarriorInputActionConfig 구조체의 array
	TArray<FCharacterInput> NativeInputActions; //TArray로 InputAction을 여러개 넣을 수 있는 공간 확보
};
