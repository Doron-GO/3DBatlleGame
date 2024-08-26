#include "DeltaTime.h"
#include<DxLib.h>

void DeltaTime::Update()
{	
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = 	static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	if (deltaTime_>0.05f)
	{
		deltaTime_ = 0.05f;
	}
	preTime_ = nowTime;
	elapsedTime_ += deltaTime_;

}

const float DeltaTime::GetDeltaTime()const
{
	return deltaTime_;
}

const float DeltaTime::GetElapsedTime()const
{
	return elapsedTime_;
}

void DeltaTime::SetStart()
{
 	preTime_ = std::chrono::system_clock::now();
}

void DeltaTime::Reset()
{
	oldTime_ = static_cast<float>(GetNowHiPerformanceCount());
	elapsedTime_ = 0.0f;

}

DeltaTime::DeltaTime() :oldTime_(0.0f), deltaTime_(0.000001f), elapsedTime_(0.0f)
{

}

DeltaTime::~DeltaTime()
{
}
