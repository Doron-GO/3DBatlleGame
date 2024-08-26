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
	// �V���O���g���Q��
	ResourceManager& resMng_;

	Transform transform_;

};

