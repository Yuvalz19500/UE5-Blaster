#include "BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get reference to EnhancedInputComponent"));

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetWorld()->GetFirstLocalPlayerFromController());
	checkf(InputSubsystem, TEXT("Unable to get reference to the EnhancedInputLocalPlayerSubsystem"));

	checkf(InputMappingContext, TEXT("InputMappingContext is not set"));
	InputSubsystem->AddMappingContext(InputMappingContext, 0);

	if (ActionMove)
	{
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ThisClass::HandleMove);
	}

	if (ActionLook)
	{
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ThisClass::HandleLook);
	}

	if (ActionJump)
	{
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ThisClass::HandleJump);
	}
}

void ABlasterCharacter::HandleMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MoveVector = InputActionValue.Get<FVector2D>();
	if (Controller && MoveVector.Length() != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector ForwardDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		const FVector RightDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
	}
}

void ABlasterCharacter::HandleLook(const FInputActionValue& InputActionValue)
{
	const FVector2D LookVector = InputActionValue.Get<FVector2D>();
	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}

void ABlasterCharacter::HandleJump()
{
}
