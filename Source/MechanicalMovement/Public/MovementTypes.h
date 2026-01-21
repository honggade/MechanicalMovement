#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MovementTypes.generated.h"


UCLASS()
class MECHANICALMOVEMENT_API UMovementTypes : public UObject
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	None UMETA(DisplayName = "无"),
	SinWave UMETA(DisplayName = "正弦波上下浮动"),
	Circular UMETA(DisplayName = "圆周运动"),
	Pendulum UMETA(DisplayName = "钟摆运动"),
	Pulse UMETA(DisplayName = "呼吸"),
	FigureEight UMETA(DisplayName = "八字形运动")
};
