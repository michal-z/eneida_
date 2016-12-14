// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "eneidaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENEIDA_API AeneidaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
};
