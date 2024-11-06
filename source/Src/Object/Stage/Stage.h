#pragma once
#include<vector>
#include"../../Manager/EffectManager.h"
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

	//�G�t�F�N�g�Đ�
	std::unique_ptr<EffectManager> effectManager_;

	std::vector<VECTOR> sPos_;
};

