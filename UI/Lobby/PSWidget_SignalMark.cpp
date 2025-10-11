// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_SignalMark.h"
#include "Animation/WidgetAnimation.h"

void UPSWidget_SignalMark::InitData()
{
	Super::InitData();

	if(Jump_Animation)
	{
		PlayAnimation(Jump_Animation, 0.0f, 0);
	}
}

void UPSWidget_SignalMark::SyncAnimation()
{
	if (Jump_Animation)
	{
		const float AnimationLength = Jump_Animation->GetEndTime() - Jump_Animation->GetStartTime();
        
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		const float AnimationOffset = FMath::Fmod(CurrentTime, AnimationLength);

		PlayAnimation(Jump_Animation, AnimationOffset, 0);
	}
}