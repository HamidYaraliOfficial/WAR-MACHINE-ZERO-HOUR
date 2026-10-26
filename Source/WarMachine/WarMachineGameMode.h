#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarMachineGameMode.generated.h"

/** Root gameplay mode: boots the campaign subsystems and spawns the player's operator. */
UCLASS()
class AWarMachineGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWarMachineGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
