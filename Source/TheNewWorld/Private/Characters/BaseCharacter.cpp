// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "WeaponSystem/WeaponMaster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseCharacter::MoveFront(float Value)
{
}

void ABaseCharacter::MoveRight(float Value)
{
}

void ABaseCharacter::LookUp(float Value)
{
}

void ABaseCharacter::LookRight(float Value)
{
}

void ABaseCharacter::OnRep_Weapons()
{
}

void ABaseCharacter::OnRep_CurrentWeaponINDEX()
{
}

void ABaseCharacter::SR_Interact_Implementation(AActor *Target, ABaseCharacter *Interactor)
{
}

void ABaseCharacter::MC_SetCurrentWeaponMesh_Implementation(USkeletalMesh *NewMesh, FName SocketName)
{
}

void ABaseCharacter::SR_SetCurrentWeaponMesh_Implementation(USkeletalMesh *NewMesh, FName SocketName)
{
}

void ABaseCharacter::MC_SetWeaponAtINDEX_Implementation(UWeaponMaster *Weapon, int32 INDEX)
{
}

void ABaseCharacter::SR_SetWeaponAtINDEX_Implementation(UWeaponMaster *Weapon, int32 INDEX)
{
}

void ABaseCharacter::MC_SwitchWeapons_Implementation(int32 INDEX)
{
}

void ABaseCharacter::SR_SwitchWeapons_Implementation(int32 INDEX)
{
}

UWeaponMaster *ABaseCharacter::GetWeaponAtINDEX(int32 INDEX)
{
    return nullptr;
}

bool ABaseCharacter::SetWeaponAtINDEX(UWeaponMaster *Weapon, int32 INDEX)
{
    return false;
}

UWeaponMaster *ABaseCharacter::GetCurrentWeapon()
{
    return nullptr;
}

bool ABaseCharacter::SetCurrentWeapon(UWeaponMaster *Weapon)
{
    return false;
}

void ABaseCharacter::SetCurrentWeaponMesh(USkeletalMesh *NewMesh, FName SocketName)
{
}

void ABaseCharacter::SwitchWeapons(int32 INDEX)
{
}

void ABaseCharacter::SpawnWeapon(TSubclassOf<UWeaponMaster> WeaponToSpawn)
{
}
