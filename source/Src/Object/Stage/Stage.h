#pragma once
#include"../Comon/Transform.h"

class ResourceManager;

class Stage
{
public:

	Stage();
	~Stage();

	//描画
	void Draw();

	//当たり判定の取得
	Collider* GetCollider();

private:
	// シングルトン参照
	ResourceManager& resMng_;

	//トランスフォーム
	Transform transform_;

};

