// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementTypes.h"
#include "Components/ActorComponent.h"
#include "MovementGeneratorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MECHANICALMOVEMENT_API UMovementGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovementGeneratorComponent();
	
    	
	// 暴露给编辑器的运动类型开关
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanical Movement")
	EMovementType MovementType = EMovementType::None;

	// 基础运动参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanical Movement")
	float Speed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanical Movement")
	float Amplitude = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanical Movement")
	float Lerptime = 1.5f;
	
	float CurrentAmplitude = Amplitude;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanical Movement|Rotation")
	bool bEnableRotation = false;

	// 使用 FRotator 可以分别设置偏航、俯仰、翻滚的速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanical Movement|Rotation", meta = (EditCondition = "bEnableRotation"))
	FRotator RotationVelocity = FRotator(0.0f, 90.0f, 0.0f); // 默认每秒转90度
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanical Movement")
	FVector MovementDirection = FVector(1.0f, 0.0f, 0.0f); // 默认 X 轴
	
protected:
	virtual void BeginPlay() override;
	
	// 初始坐标
	FVector InitialLocation;
	// 累计运行时间
	float RunningTime =0.0f;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
