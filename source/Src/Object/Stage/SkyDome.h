#pragma once
#include "../Common/Transform.h"

class SkyDome
{
public:


	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	SkyDome(void);	//プレイヤーに追従する場合はこっち
	~SkyDome(void);

	void Update(void);
	void Draw(void);
	void Release(void);

private:
	// 状態
	STATE state_;

	// モデル制御の基本情報
	Transform transform_;

};
