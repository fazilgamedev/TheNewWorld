// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/ArmsAnimInst.h"
#include "Characters/BaseCharacter.h"

UArmsAnimInst::UArmsAnimInst()
{

}

void UArmsAnimInst::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    CharacterREF = Cast<ABaseCharacter>(TryGetPawnOwner());
    if(!CharacterREF) return;
}

void UArmsAnimInst::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);
    
    if(!CharacterREF) CharacterREF = Cast<ABaseCharacter>(TryGetPawnOwner());
}
