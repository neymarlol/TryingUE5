// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRYINGUE5_API UParkourMovementComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	class UCharacterMovementComponent* CharMovementComp;

	UPROPERTY()
	class ACharacter* Character;

	// Dash speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeed;

	// Dash duration
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDuration;

	// Dash cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCooldown;

	// Airjump strength and direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AirJumpImpulse;

	// Overide or add to jump force when in air
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAirJumpVelocityOverride;


private:
	// Is dashing flag
	bool bIsDashing;

	// Can dash flag
	bool bCanDash;

	// Can air jump flag
	bool bCanAirJump;

	// Original grav scale of char movement comp
	float OriginalGravityScale;


	// Timer handle for dash duration
	UPROPERTY()
	FTimerHandle TimerHandle_DashDuration;

	// Timer handle for dash cooldown
	UPROPERTY()
	FTimerHandle TimerHandle_DashCooldown;

	// Stop dashing function
	UFUNCTION()
	void StopDashing();

	

	UFUNCTION()
	void Landed(const FHitResult& Hit);
	

public:	
	// Sets default values for this component's properties
	UParkourMovementComponent();

	// Dash function
	UFUNCTION(BlueprintCallable)
	void Dash();

	// Reset dash function
	UFUNCTION(BlueprintCallable)
	void ResetDash();

	//DoubleJump
	UFUNCTION(BlueprintCallable)
	void AirJump();

	// Reset airjump
	UFUNCTION(BlueprintCallable)
	void AirJumpReset();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
