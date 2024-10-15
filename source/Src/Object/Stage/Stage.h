#pragma once
#include"../Common/Transform.h"

class ResourceManager;

class Stage
{
public:

	Stage(void);
	~Stage(void);

	//描画
	void Draw(void);

	//当たり判定の取得
	Collider* GetCollider(void);

private:
	// シングルトン参照
	ResourceManager& resMng_;

	//トランスフォーム
	Transform transform_;

};

