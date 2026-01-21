#include "MovementGeneratorComponent.h"



UMovementGeneratorComponent::UMovementGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UMovementGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()){ InitialLocation = GetOwner()->GetActorLocation(); }
}


void UMovementGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bEnableRotation)
	{
		// 计算这一帧应该转多少度：速度 * DeltaTime
		// AddActorLocalRotation 会自动处理每一帧的增量叠加
		GetOwner()->AddActorLocalRotation(RotationVelocity * DeltaTime);
	}
	// 如果这个组件找不到爹，或者没有设置运动类型，就不动
	if (!GetOwner()||MovementType==EMovementType::None){return;}
	
	// 使用 FMath::Clamp 确保 RunningTime 超过 Lerptime 后，Alpha 停留在 1.0
	float Alpha = FMath::Clamp(RunningTime / Lerptime, 0.0f, 1.0f);
	
	// 每一帧累加 DeltaTime，如后续想要实现慢动作，可以加一个系数
	RunningTime += DeltaTime;
	
	// A 是起始值(0)，B 是目标值(你面板里填的那个 Amplitude)
	CurrentAmplitude = FMath::Lerp(0.0f, Amplitude, Alpha);
	
	// 归一化方向向量，防止因为向量长度导致的速度不一
	FVector SafeDirection = MovementDirection.GetSafeNormal();
	FVector NewLocation = InitialLocation;
	float Offset = 0.0f;
	
	switch (MovementType)
	{
	case EMovementType::SinWave:
		{
			// 标准正弦波上下浮动
			Offset = CurrentAmplitude * FMath::Sin(RunningTime * Speed);
			// 应用方向位移
			NewLocation = InitialLocation + (SafeDirection * Offset);
			break;
		}

	case EMovementType::Circular:
		{
			// 圆周运动：SafeDirection 此时代表圆平面的“法线”（轴心）
			FVector U, V;
			SafeDirection.FindBestAxisVectors(U, V); 

			// 计算圆周坐标：x = cos(t), y = sin(t)
			// 这样物体会绕着 InitialLocation 旋转，半径为 CurrentAmplitude
			FVector CircleOffset = (U * CurrentAmplitude * FMath::Cos(RunningTime * Speed)) + 
								   (V * CurrentAmplitude * FMath::Sin(RunningTime * Speed));
            
			// 如果你想让物体从初始位置作为圆周的一个点开始，需要减去初始时刻的偏移
			// 但通常机械运动让 InitialLocation 作为圆心最符合直觉
			NewLocation = InitialLocation + CircleOffset;
			break;
		}
	
	case EMovementType::Pendulum:
		{
			// 钟摆运动：实际上是角度的 Sin 摆动，但这里我们用位移模拟弧线摆动
			// 公式：在 SafeDirection 垂直的平面上进行弧形位移
			FVector U, V;
			SafeDirection.FindBestAxisVectors(U, V); 
        
			float Angle = FMath::Sin(RunningTime * Speed);
			// U 是摆动方向，SafeDirection 的反向是重力指向（简化模拟）
			FVector PendulumOffset = (U * CurrentAmplitude * FMath::Sin(Angle)) + 
									 (SafeDirection * -CurrentAmplitude * FMath::Cos(Angle));
        
			// 注意：这里我们将 InitialLocation 视为挂钩点
			NewLocation = InitialLocation + PendulumOffset;
			break;
		}
	
	case EMovementType::Pulse:
		{
			// 脉冲/呼吸效果：修改的是 Scale（缩放），所以这里位置保持不变
			// 逻辑：计算一个缩放系数，并在 switch 之后应用
			float PulseScale = 1.0f + (CurrentAmplitude / 100.0f) * FMath::Sin(RunningTime * Speed);
			GetOwner()->SetActorScale3D(FVector(PulseScale));
        
			NewLocation = InitialLocation; // 位置维持原点
			break;
		}
	
	case EMovementType::FigureEight:
		{
			// 八字形运动 (Lissajous Curve)
			FVector U, V;
			SafeDirection.FindBestAxisVectors(U, V);
        
			float TimeValue = RunningTime * Speed;
			// 典型的 1:2 频率比形成八字
			FVector EightOffset = (U * CurrentAmplitude * FMath::Cos(TimeValue)) + 
								  (V * (CurrentAmplitude * 0.5f) * FMath::Sin(TimeValue * 2.0f));
        
			NewLocation = InitialLocation + EightOffset;
			break;
		}
		
	default:
		break;
	}
	
	GetOwner()->SetActorLocation(NewLocation);
}

