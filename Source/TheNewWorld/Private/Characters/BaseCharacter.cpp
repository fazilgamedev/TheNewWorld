// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "WeaponSystem/WeaponMaster.h"
#include "WeaponSystem/WeaponPickup.h"
#include "PickupSystem/PickupMaster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "PickupSystem/Interfaces/InteractInterface.h"
#include "AnimInstances/ArmsAnimInst.h"
#include "AnimInstances/BodyAnimInst.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	WeaponFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponFP"));
	WeaponMagFP = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMagFP"));
	WeaponTP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponTP"));
	WeaponMagTP = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMagTP"));

	Arms->SetupAttachment(GetRootComponent());
	Arms->SetOnlyOwnerSee(true);
	Arms->SetCastShadow(false);

	WeaponFP->SetupAttachment(Arms);
	WeaponFP->SetOnlyOwnerSee(true);
	WeaponFP->SetCastShadow(false);

	WeaponMagFP->SetupAttachment(WeaponFP);
	WeaponMagFP->SetOnlyOwnerSee(true);
	WeaponMagFP->SetCastShadow(false);

	WeaponTP->SetupAttachment(GetMesh());
	WeaponTP->SetOwnerNoSee(true);
	WeaponTP->SetCastHiddenShadow(true);

	WeaponMagTP->SetupAttachment(WeaponTP);
	WeaponMagTP->SetOwnerNoSee(true);
	WeaponMagTP->SetCastHiddenShadow(true);

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCastHiddenShadow(true);

	Weapons.Init(nullptr, 2);
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	Camera = Cast<UCameraComponent>(AddComponentByClass(UCameraComponent::StaticClass(), true, FTransform::Identity, false));

	Camera->SetFieldOfView(120.f);
	Camera->bUsePawnControlRotation = true;

	Camera->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("head"));
	
	ArmsAnimInst = Cast<UArmsAnimInst>(Arms->GetAnimInstance());

	BodyAnimInst = Cast<UBodyAnimInst>(GetMesh()->GetAnimInstance());

	SwitchWeapons(-1);

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Speed = GetVelocity().Size2D();
	float TargetFreq = 0.f;
	FVector TargetAmp = FVector::ZeroVector;
	if(Speed > 400.f){
		TargetFreq = 13.96f;
		TargetAmp = FVector(2.f, 2.f, 2.f);
	}else if(Speed > 3.f){
		TargetFreq = 10.47f;
		TargetAmp = FVector(.5f, .5f, .5f);
	}else{
		TargetFreq = 3.14;
		TargetAmp = FVector(.12f, .12f, .12f);
	}
	CurrentAmp = FMath::VInterpTo(CurrentAmp, TargetAmp, DeltaTime, 5.f);
	CurrentPhase += DeltaTime * TargetFreq;
	if(CurrentPhase > 6.28f) CurrentPhase -= 6.28f;
	WalkVector = FVector(FMath::Sin(CurrentPhase) * CurrentAmp.X, FMath::Cos(CurrentPhase) * CurrentAmp.Y, FMath::Sin(CurrentPhase * 2.f) * CurrentAmp.Z);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFront", this, &ABaseCharacter::MoveFront);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ABaseCharacter::LookRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseCharacter::Interacting);
	PlayerInputComponent->BindAction("Primary", IE_Pressed, this, &ABaseCharacter::SwitchPrimary);
	PlayerInputComponent->BindAction("Secondary", IE_Pressed, this, &ABaseCharacter::SwitchSecondary);
	PlayerInputComponent->BindAction("Unarmed", IE_Pressed, this, &ABaseCharacter::SwitchUnarmed);
	PlayerInputComponent->BindAxis("ADS", this, &ABaseCharacter::ADS);

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Weapons);
	DOREPLIFETIME(ABaseCharacter, CurrentWeaponINDEX);
}

void ABaseCharacter::MoveFront(float Value)
{
	if(Value != 0.f) AddMovementInput(GetActorForwardVector(), Value);
	if(ArmsAnimInst) FrontMove = ArmsAnimInst->AimAlpha > .5f ? Value * .5f : Value;
	else FrontMove = Value;
}

void ABaseCharacter::MoveRight(float Value)
{	
	AddMovementInput(GetActorRightVector(), Value);
	SideMove = Value * 2.f;
}

void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * V_Sensitivity);
}

void ABaseCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value * H_Sensitivity);
}

void ABaseCharacter::OnRep_Weapons()
{
}

void ABaseCharacter::OnRep_CurrentWeaponINDEX()
{
	if(CurrentWeaponINDEX == -1) SetCurrentWeaponMesh(nullptr, nullptr, TEXT(""));
	else{
		if(GetCurrentWeapon()) SetCurrentWeaponMesh(GetCurrentWeapon()->WeaponMesh, GetCurrentWeapon()->MagazineMesh,GetCurrentWeapon()->SocketToAttach);
		else SetCurrentWeaponMesh(nullptr, nullptr, TEXT(""));
	}
}

void ABaseCharacter::SR_Interact_Implementation(AActor *Target, ABaseCharacter *Interactor)
{
	IInteractInterface::Execute_Interact(Target, Interactor);
}

void ABaseCharacter::MC_SetCurrentWeaponMesh_Implementation(USkeletalMesh *NewMesh, UStaticMesh *NewMagMesh, FName SocketName)
{
	if(!(WeaponTP && WeaponFP)) return;
	if(NewMesh){
		WeaponTP->SetSkeletalMesh(NewMesh, true);
		WeaponTP->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		WeaponFP->SetSkeletalMesh(NewMesh, true);
		WeaponFP->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);		
		if(NewMagMesh){
			WeaponMagTP->SetStaticMesh(NewMagMesh);
			WeaponMagFP->SetStaticMesh(NewMagMesh);
		}else{
			WeaponMagTP->SetStaticMesh(nullptr);
			WeaponMagFP->SetStaticMesh(nullptr);
		}
	}else{
		WeaponTP->SetSkeletalMesh(nullptr);
		WeaponFP->SetSkeletalMesh(nullptr);
		WeaponMagTP->SetStaticMesh(nullptr);
		WeaponMagFP->SetStaticMesh(nullptr);
	}
}

void ABaseCharacter::SR_SetCurrentWeaponMesh_Implementation(USkeletalMesh *NewMesh, UStaticMesh *NewMagMesh, FName SocketName)
{
	MC_SetCurrentWeaponMesh(NewMesh, NewMagMesh, SocketName);
}

void ABaseCharacter::MC_SetWeaponAtINDEX_Implementation(UWeaponMaster *Weapon, int32 INDEX)
{
	if(Weapons.IsValidIndex(INDEX)) Weapons[INDEX] = Weapon;
}

void ABaseCharacter::SR_SetWeaponAtINDEX_Implementation(UWeaponMaster *Weapon, int32 INDEX)
{
	MC_SetWeaponAtINDEX(Weapon, INDEX);
}

void ABaseCharacter::MC_SwitchWeapons_Implementation(int32 INDEX)
{
	if(CurrentWeaponINDEX == INDEX) return;
	CurrentWeaponINDEX = INDEX;
	if(HasAuthority()) OnRep_CurrentWeaponINDEX();
}

void ABaseCharacter::SR_SwitchWeapons_Implementation(int32 INDEX)
{
	MC_SwitchWeapons(INDEX);
}

void ABaseCharacter::Interacting()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors, APickupMaster::StaticClass());
	if(Actors.Num() > 0 && Actors[0] && Actors[0]->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())){
		if(HasAuthority()) 	IInteractInterface::Execute_Interact(Actors[0], this);
		else SR_Interact(Actors[0], this);
	}
}

void ABaseCharacter::SwitchPrimary()
{
	SwitchWeapons(0);
}

void ABaseCharacter::SwitchSecondary()
{
	SwitchWeapons(1);
}

void ABaseCharacter::SwitchUnarmed()
{
	SwitchWeapons(-1);
}

UWeaponMaster *ABaseCharacter::GetWeaponAtINDEX(int32 INDEX)
{
    return Weapons.IsValidIndex(INDEX) ? Weapons[INDEX] : nullptr;
}

bool ABaseCharacter::SetWeaponAtINDEX(UWeaponMaster *Weapon, int32 INDEX)
{
	if(Weapons.IsValidIndex(INDEX)){
		if(HasAuthority()) MC_SetWeaponAtINDEX(Weapon, INDEX);
		else SR_SetWeaponAtINDEX(Weapon, INDEX);
		return true;
	}
	return false;
}

UWeaponMaster *ABaseCharacter::GetCurrentWeapon()
{
    return Weapons.IsValidIndex(CurrentWeaponINDEX) ? GetWeaponAtINDEX(CurrentWeaponINDEX) : nullptr;
}

bool ABaseCharacter::SetCurrentWeapon(UWeaponMaster *Weapon)
{
	if(!Weapon){
		if(SetWeaponAtINDEX(nullptr, CurrentWeaponINDEX)){
			SetCurrentWeaponMesh(nullptr, nullptr,TEXT(""));
			return true;
		}
		return false;
	}else{
		if(SetWeaponAtINDEX(Weapon, CurrentWeaponINDEX)){ 
			SetCurrentWeaponMesh(Weapon->WeaponMesh, Weapon->MagazineMesh, Weapon->SocketToAttach);
			return true;
		}
		return false;
	}
}

void ABaseCharacter::SetCurrentWeaponMesh(USkeletalMesh *NewMesh, UStaticMesh *NewMagMesh, FName SocketName)
{
	if(HasAuthority()) MC_SetCurrentWeaponMesh(NewMesh, NewMagMesh, SocketName);
	else SR_SetCurrentWeaponMesh(NewMesh, NewMagMesh, SocketName);
}

void ABaseCharacter::SwitchWeapons(int32 INDEX)
{
	if(HasAuthority()) MC_SwitchWeapons(INDEX);
	else SR_SwitchWeapons(INDEX);
}

bool ABaseCharacter::SpawnWeapon(TSubclassOf<UWeaponMaster> WeaponToSpawn)
{
	if(!WeaponToSpawn) return false;
	if(!Weapons.IsValidIndex(CurrentWeaponINDEX)) SwitchWeapons(0);
	UWeaponMaster* OldWeapon = GetCurrentWeapon();
	if(OldWeapon){
		GetWorld()->SpawnActor<AWeaponPickup>(OldWeapon->PickupClass, GetActorLocation() + GetActorForwardVector() * 100.f, FRotator(30.f, 0.f, 0.f));
		OldWeapon->DestroyComponent();
		SetCurrentWeapon(nullptr);
	}
	UWeaponMaster* NewComp = Cast<UWeaponMaster>(AddComponentByClass(WeaponToSpawn, true, FTransform::Identity, false));
	return SetCurrentWeapon(NewComp);
}

void ABaseCharacter::ADS(float Value)
{
	if(!GetCurrentWeapon()) return;
	if(ArmsAnimInst) ArmsAnimInst->AimAlpha = FMath::FInterpTo(ArmsAnimInst->AimAlpha, Value, GetWorld()->GetDeltaSeconds(), 10.f);
	//Camera->SetFieldOfView(FMath::FInterpTo(90.f, Value ? GetCurrentWeapon()->ADSFOV : 90.f, GetWorld()->GetDeltaSeconds(), 10.f));
}
