// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArmsAnimInst.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UArmsAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UArmsAnimInst();

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:

	UPROPERTY()
	ABaseCharacter* CharacterREF;

//////////////////////////////////////////////////////////////////



public:

	
	
};
