// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BodyAnimInst.h"
#include "Characters/BaseCharacter.h"

UBodyAnimInst::UBodyAnimInst()
{
}

void UBodyAnimInst::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    CharacterREF = Cast<ABaseCharacter>(TryGetPawnOwner());
    if(!CharacterREF) return;
}

void UBodyAnimInst::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if(!CharacterREF) return;

    Speed = CharacterREF->GetVelocity().Size2D();

    Direction = CalculateDirection(CharacterREF->GetVelocity(), CharacterREF->GetActorRotation());
}
