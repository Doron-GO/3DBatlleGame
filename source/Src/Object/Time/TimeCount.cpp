#include "TimeCount.h"
#include<DxLib.h>

TimeCount::TimeCount(CheckPoint& CheckPoint):checkPoint_(CheckPoint),oldTime_(0.0f),
deltaTime_(0.000001f),elapsedTime_(0.0f), nowTime_(0.0f), startFlag_(false),_update(&TimeCount::GameUpdate), endFlag_(false)
{
}

TimeCount::~TimeCount()
{
}

void TimeCount::Update(float startime)
{
	(this->*_update)( startime);
}

void TimeCount::Draw()
{
}


void TimeCount::GameUpdate(float startime)
{
	if (startime >= 2.0f && (!startFlag_))
	{
		SetStart();
		startFlag_ = true;
	}
	Count();
}


void TimeCount::SetStart()
{
	oldTime_ =static_cast<float>( GetNowHiPerformanceCount());
}

bool TimeCount::IsEnd()
{
	return endFlag_;
}

float TimeCount::ElapsedTime()
{
	return elapsedTime_;
}

void TimeCount::Count()
{
	nowTime_ =static_cast<float>( GetNowHiPerformanceCount());
	deltaTime_ = (nowTime_ - oldTime_) / 1000000.0f;
	oldTime_ = nowTime_;
	elapsedTime_ += deltaTime_;
}

