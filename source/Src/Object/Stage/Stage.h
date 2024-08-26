#pragma once
#include"../Comon/Transform.h"

class ResourceManager;

class Stage
{
public:

	Stage();
	~Stage();

	void Draw();
	Collider* GetCollider();

private:
	// シングルトン参照
	ResourceManager& resMng_;

	Transform transform_;

};

