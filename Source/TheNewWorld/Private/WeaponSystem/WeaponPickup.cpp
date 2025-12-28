// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/WeaponPickup.h"
#include "Characters/BaseCharacter.h"
#include "WeaponSystem/WeaponMaster.h"

void AWeaponPickup::Interact_Implementation(ABaseCharacter *Interactor)
{
    SR_Interact(Interactor, WeaponToSpawn);
}

void AWeaponPickup::SR_Interact_Implementation(ABaseCharacter *Interactor, TSubclassOf<UWeaponMaster> Weapon)
{
    if(Interator->SpawnWeapon(Weapon)){
        Destroy();
    }
}