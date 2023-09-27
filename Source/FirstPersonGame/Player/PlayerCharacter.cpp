#include "FirstPersonGame/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword Mesh"));
	SwordMesh->SetupAttachment(GetMesh(), FName("SwordSocket"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
//Bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Keyboard
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);


	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::SprintEnd);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	//Mouse
	PlayerInputComponent->BindAxis("TurnCamera", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	//Attack
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::StartAttack);
	
}

//Movement
void APlayerCharacter::MoveForward(float InputValue)
{
	FVector ForwardDirection = GetActorForwardVector();
	AddMovementInput(ForwardDirection, InputValue);
}

void APlayerCharacter::MoveRight(float InputValue)
{
	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, InputValue);
}

void APlayerCharacter::Turn(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void APlayerCharacter::LookUp(float InputValue)
{
	AddControllerPitchInput(InputValue);
}

void APlayerCharacter::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	
}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::StartCrouch()
{
	ACharacter::Crouch();
}

void APlayerCharacter::EndCrouch()
{
	ACharacter::UnCrouch();
}

//Attack
void APlayerCharacter::StartAttack()
{
	//Call attack animation
	if (AttackAnimation && !bIsAttacking)
	{
		GetMesh()->PlayAnimation(AttackAnimation, false);
		bIsAttacking = true;
	}
}

void APlayerCharacter::LineTrace()
{
	//Deal Damage to enemies in range
	FVector StartLocation = SwordMesh->GetSocketLocation(FName("Start"));
	FVector EndLocation = SwordMesh->GetSocketLocation(FName("End"));
	//Get Socket Locations

	//Setup Linetrace
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	//LineTrace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	//DebugLines
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);

	if (HitResult.bBlockingHit)
	{
		AActor* ActorHit = HitResult.GetActor();
		ActorHit->Destroy();

		
	}
}

