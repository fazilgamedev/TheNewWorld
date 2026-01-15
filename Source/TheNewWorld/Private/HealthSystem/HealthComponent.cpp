// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSystem/HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::Heal(float Amount)
{
	if (bIsDead) return 0.f;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
	return CurrentHealth;
}

bool UHealthComponent::TakeDamage(FDamageInfo DamageInfo)
{
	if(bIsDead) return false;
	CurrentHealth = CurrentHealth - DamageInfo.Amount;
	if(CurrentHealth <= 0.f){
		bIsDead = true;
		OnDeath.Broadcast();
	}
	return true;
}
