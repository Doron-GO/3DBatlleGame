#pragma once
#include"../Comon/Transform.h"

class ResourceManager;

class Stage
{
public:

	Stage();
	~Stage();

	//�`��
	void Draw();

	//�����蔻��̎擾
	Collider* GetCollider();

private:
	// �V���O���g���Q��
	ResourceManager& resMng_;

	//�g�����X�t�H�[��
	Transform transform_;

};

