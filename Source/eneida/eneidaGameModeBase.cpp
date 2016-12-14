// Fill out your copyright notice in the Description page of Project Settings.

#include "eneida.h"
#include "eneidaGameModeBase.h"




void AeneidaGameModeBase::BeginPlay()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("eneida"));
	}
}
