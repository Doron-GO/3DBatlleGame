#pragma once
#include "../Common/Transform.h"

class SkyDome
{
public:

	static constexpr float SCALE = 0.5f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	SkyDome(const Transform& syncTransform);	//プレイヤーに追従する場合はこっち
	~SkyDome(void);

	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// プレイヤーの情報
	const Transform& syncTransform_;

	// 状態
	STATE state_;

	// モデル制御の基本情報
	Transform transform_;

};
