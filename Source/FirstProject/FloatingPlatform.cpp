// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include <Components/StaticMeshComponent.h>
#include <TimerManager.h>

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.0f);
	EndPoint = FVector(0.0f);
	InterpSpeed = 4.0f;
	InterpTime = 1.0f;
	bInterping = false;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	bInterping = false;

	Distance = (EndPoint - StartPoint).Size();

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(Interp);

		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= 1.0f)
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}
	
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector& V1, FVector& V2)
{
	FVector Temp = V1;
	V1 = V2;
	V2 = Temp;
}



