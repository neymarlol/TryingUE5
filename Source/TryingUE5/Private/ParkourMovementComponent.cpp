// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"

//#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UParkourMovementComponent::UParkourMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DashSpeed = 2000.0f;
	DashDuration = 0.2f;
	DashCooldown = 1.0f;
	AirJumpImpulse = { 0.0f, 0.0f, 1000.0f };
	bIsDashing = false;
	bCanDash = true;
	bCanAirJump = true;
	bAirJumpVelocityOverride = true;
	DashCameraLag = 10.0f;
	InAirCameraLag = 10.0f;
	GroundedCameraLag = 20.0f;
	// ...
}




// Called when the game starts
void UParkourMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get needed components
	Character = Cast<ACharacter>(GetOwner());
	CharMovementComp = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();

	if (Character && CharMovementComp)
	{	
		// Bind to landed and use it to jump reset
		Character->LandedDelegate.AddDynamic(this, &UParkourMovementComponent::Landed);

		// Set original grav scale
		OriginalGravityScale = CharMovementComp->GravityScale;

		// Set spring arm component
		SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>();

		// Set base camera lag speed
		SpringArm->bEnableCameraLag = true;
	}
	
	
	// ...
	
}


// Called every frame
void UParkourMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDashing && CharMovementComp)
	{
		//CharMovementComp->Velocity += GetOwner()->GetActorForwardVector() * DashSpeed * DeltaTime;
	}

	// Camera lag enable or disable
	if (SpringArm && CharMovementComp)
	{
		if (bIsDashing) { SpringArm->CameraLagSpeed = DashCameraLag; }
		else if (CharMovementComp->IsFalling()) { SpringArm->CameraLagSpeed = InAirCameraLag; }
		else if (!CharMovementComp->IsFalling()) { SpringArm->CameraLagSpeed = GroundedCameraLag; }
	}

	// ...

}

void UParkourMovementComponent::Dash(UNiagaraComponent* NiagaraComp)
{
	if (bCanDash && CharMovementComp)
	{
		// Play animation and sound here

		// Set flags
		bIsDashing = true;
		bCanDash = false;
		
		// Stop momentum
		CharMovementComp->StopMovementImmediately();
		CharMovementComp->GravityScale = 0.0f;

		// Stop reading player input during dash
		PlayerController = Cast<APlayerController>(Character->GetController());
		if (PlayerController) { Character->DisableInput(PlayerController); }

		// Set camera lag
		if (SpringArm) { SpringArm->CameraLagSpeed = DashCameraLag; }

		// Set dash velocity
		CharMovementComp->Velocity += GetOwner()->GetActorForwardVector() * DashSpeed;

		// Use effect if one exists
		CurrentNiagaraCompForDash = NiagaraComp;
		if (CurrentNiagaraCompForDash) { CurrentNiagaraCompForDash->SetActive(true, true); }

		// Start timers
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DashDuration, this, &UParkourMovementComponent::StopDashing, DashDuration);
		
	}
}

void UParkourMovementComponent::AirJump()
{
	// Make sure can air jump and charmovement pointer is valid
	if (!bCanAirJump || !CharMovementComp)
	{
		return;
	}

	if (!CharMovementComp->IsFalling()) { return; }

	// Jump velocity
	Character->LaunchCharacter(AirJumpImpulse, false, bAirJumpVelocityOverride);
	// Set flag to false
	bCanAirJump = false;

}

void UParkourMovementComponent::StopDashing()
{	
	// Make sure dash is active
	if (!bIsDashing)
	{
		return;
	}

	// Reset Camera lag
	if (SpringArm) { SpringArm->CameraLagSpeed = InAirCameraLag; }

	// Stop momentum
	CharMovementComp->StopMovementImmediately();

	// Setting gravity scale back to OG value
	CharMovementComp->GravityScale = OriginalGravityScale;

	// Allow player inputs
	PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController) { Character->EnableInput(PlayerController); }

	// Stop VFX
	if (CurrentNiagaraCompForDash) { CurrentNiagaraCompForDash->SetActive(false, false); }

	// Clear timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DashDuration);
	
	// Start timer Dash Cooldown
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DashCooldown, this, &UParkourMovementComponent::ResetDash, DashCooldown);

	// Set flags
	bIsDashing = false;
}

void UParkourMovementComponent::ResetDash()
{
	// Clear timer and flag
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DashCooldown);
	bCanDash = true;
}

void UParkourMovementComponent::Landed(const FHitResult& Hit)
{
	// Call jump reset function up on landing
	AirJumpReset();
	
}

void UParkourMovementComponent::AirJumpReset()
{
	// reset jump
	bCanAirJump = true;
}



