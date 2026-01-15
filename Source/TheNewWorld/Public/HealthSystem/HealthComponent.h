// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthSystem/DamageInfo.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageResponse, EDamageResponse, DamageResponse);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THENEWWORLD_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnDamageResponse OnDamageResponse;	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float Heal(float Amount);

	UFUNCTION(BlueprintCallable)
	bool TakeDamage(FDamageInfo DamageInfo);	
	
};
