#include "DeltaTime.h"
#include<DxLib.h>

void DeltaTime::Update(void)
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

const float DeltaTime::GetDeltaTime(void)const
{
	return deltaTime_;
}

const float DeltaTime::GetElapsedTime(void)const
{
	return elapsedTime_;
}

void DeltaTime::SetStart(void)
{
 	preTime_ = std::chrono::system_clock::now();
}

void DeltaTime::Reset(void)
{
	elapsedTime_ = 0.0f;
}

DeltaTime::DeltaTime(void) : deltaTime_(0.000001f), elapsedTime_(0.0f)
{

}

DeltaTime::~DeltaTime(void)
{
}
