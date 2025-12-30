// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BodyAnimInst.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class THENEWWORLD_API UBodyAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UBodyAnimInst();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:

	UPROPERTY()
	ABaseCharacter* CharacterREF;

//////////////////////////////////////////////////////////////////



public:

	
	
};
