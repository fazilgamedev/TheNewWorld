// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class UWeaponMaster;

UCLASS()
class THENEWWORLD_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponTP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponFP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float H_Sensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float V_Sensitivity;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY()
	FTimerHandle FireHandle;

	UPROPERTY()
	FHitResult FireHitResult;

///////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void MoveFront(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void LookUp(float Value);

	UFUNCTION()
	void LookRight(float Value);

	UPROPERTY(ReplicatedUsing = OnRep_Weapons)
	TArray<UWeaponMaster*> Weapons;

	UFUNCTION()
	void OnRep_Weapons();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeaponINDEX)
	int32 CurrentWeaponINDEX;

	UFUNCTION()
	void OnRep_CurrentWeaponINDEX();

	UFUNCTION(Server, Reliable)
	void SR_Interact(AActor* Target, ABaseCharacter* Interactor);

	UFUNCTION(NetMulticast, Reliable)
	void MC_SetCurrentWeaponMesh(USkeletalMesh* NewMesh, FName SocketName);

	UFUNCTION(Server, Reliable)
	void SR_SetCurrentWeaponMesh(USkeletalMesh* NewMesh, FName SocketName);

	UFUNCTION(NetMulticast, Reliable)
	void MC_SetWeaponAtINDEX(UWeaponMaster* Weapon, int32 INDEX);

	UFUNCTION(Server, Reliable)
	void SR_SetWeaponAtINDEX(UWeaponMaster* Weapon, int32 INDEX);

	UFUNCTION(NetMulticast, Reliable)
	void MC_SwitchWeapons(int32 INDEX);
	
	UFUNCTION(Server, Reliable)
	void SR_SwitchWeapons(int32 INDEX);

	UFUNCTION()
	void Interacting();

	UFUNCTION()
	void SwitchPrimary();

	UFUNCTION()
	void SwitchSecondary();

	UFUNCTION()
	void SwitchUnarmed();

public:

	UFUNCTION()
	UWeaponMaster* GetWeaponAtINDEX(int32 INDEX);

	UFUNCTION()
	bool SetWeaponAtINDEX(UWeaponMaster* Weapon, int32 INDEX);

	UFUNCTION()
	UWeaponMaster* GetCurrentWeapon();

	UFUNCTION()
	bool SetCurrentWeapon(UWeaponMaster* Weapon);

	UFUNCTION()
	void SetCurrentWeaponMesh(USkeletalMesh* NewMesh, FName SocketName);

	UFUNCTION()
	void SwitchWeapons(int32 INDEX);

	UFUNCTION()
	bool SpawnWeapon(TSubclassOf<UWeaponMaster> WeaponToSpawn);
	

	
};
