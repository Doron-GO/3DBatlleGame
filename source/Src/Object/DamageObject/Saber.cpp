#include "Saber.h"

Saber::Saber(int playerType, Transform& transform) :DamageObject(playerType), transform_(transform)
{
	//�_���[�W�ʂ�ݒ�
	damage_ = 250.0f;
	//�G�t�F�N�g�}�l�[�W���[�𐶐�
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(SABER_STATE::HIT), HIT_EFFECT_SCARE, false, resMng_.Load(ResourceManager::SRC::HIT).handleId_);
}

void Saber::Update(VECTOR enemyPos)
{
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�G�t�F�N�g�̍X�V
	effectManager_->Update();
}

void Saber::Draw(void)
{

}

void Saber::Hit(void)
{
	//��L��������
	isActive_ = false;
	//�G�t�F�N�g�̍Đ�
	effectManager_->Play(static_cast<int>(SABER_STATE::HIT));
}

const int& Saber::GetModelId(void) const
{
	return transform_.modelId;
}


