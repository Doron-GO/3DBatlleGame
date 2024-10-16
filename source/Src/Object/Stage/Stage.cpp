#include "Stage.h"
#include<DxLib.h>
#include"../../Manager/ResourceManager.h"

Stage::Stage():resMng_(ResourceManager::GetInstance())
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	//�傫��
	transform_.scl = {250.0f,250.0f,250.0f };
	//���W
	transform_.pos = { 0.0f, -30.0f * 80.0f, 0.0f };
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	transform_.Update();
	//�R���W�����̍쐬
	transform_.MakeCollider(Collider::TYPE::STAGE);
}

Stage::~Stage(void)
{
}

void Stage::Draw(void)
{
	//�`��
	MV1DrawModel(transform_.modelId);
}

Collider* Stage::GetCollider(void)
{
	return transform_.collider;
}
