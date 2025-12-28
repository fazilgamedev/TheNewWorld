// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSystem/PickupMaster.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
APickupMaster::APickupMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	PickupModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupModel"));
	SetRootComponent(PickupModel);
	PickupModel->SetSimulatePhysics(true);
	PickupModel->SetMassOverrideInKg(FName(), 7.f, true);
	PickupModel->SetLinearDamping(.4f);
	PickupModel->SetAngularDamping(.4f);
	PickupModel->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PickupModel->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

	PickupRadius = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRadius"));
	PickupRadius->SetupAttachment(PickupModel);
	PickupRadius->SetSphereRadius(100.f);

}

// Called when the game starts or when spawned
void APickupMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupMaster::Interact_Implementation(ABaseCharacter *Interactor)
{
}
