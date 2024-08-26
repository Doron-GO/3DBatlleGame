#pragma once
#include<DxLib.h>
#include<chrono>


class DeltaTime
{
public:

	static DeltaTime& GetInstsnce()
	{
		static DeltaTime s_Instance;
		return s_Instance;
	}
	void Update();

	const float GetDeltaTime()const;
	const float GetElapsedTime()const;
	void SetStart();
	void Reset();

private:
	DeltaTime();
	~DeltaTime();
	float deltaTime_;
	std::chrono::system_clock::time_point preTime_;

	float elapsedTime_;
	float oldTime_;

};

