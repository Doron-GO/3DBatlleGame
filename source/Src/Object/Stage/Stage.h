#pragma once
#include"../Common/Transform.h"

class ResourceManager;

class Stage
{
public:

	Stage(void);
	~Stage(void);

	//�`��
	void Draw(void);

	//�����蔻��̎擾
	Collider* GetCollider(void);

private:
	// �V���O���g���Q��
	ResourceManager& resMng_;

	//�g�����X�t�H�[��
	Transform transform_;

};

