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
	//更新
	void Update(void);

	//デルタタイムの取得
	const float GetDeltaTime(void)const;

	//経過時間の取得
	const float GetElapsedTime(void)const;

	//計測の開始
	void SetStart(void);

	//経過時間のリセット
	void Reset(void);

private:

	DeltaTime(void);

	~DeltaTime(void);

	//デルタタイム
	float deltaTime_;

	//デルタタイム計測用の最新時間
	std::chrono::system_clock::time_point preTime_;
	
	//経過時間
	float elapsedTime_;

};

