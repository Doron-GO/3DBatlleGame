#include "Stage.h"
#include<DxLib.h>
#include"../../Manager/ResourceManager.h"

//�o���A�̃G�t�F�N�g�^�C�v
constexpr int EFFECT_TYPE = 0;

//�X�e�[�W�̍��W
constexpr VECTOR STAGE_POS = { 0.0f, -2400.0f,0.0f };

//�X�e�[�W�̑傫��
constexpr VECTOR STAGE_SCALE = { 250.0f, 250.0f,250.0f };

//�o���A�[�̑傫��
constexpr VECTOR BARRIER_SCALE = { 3500.0f,3500.0f,3500.0f };

//�Đ����x
constexpr float BARRIER_SPEED = 0.4f;

Stage::Stage():resMng_(ResourceManager::GetInstance())
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	//�傫��
	transform_.scl = STAGE_SCALE;
	//���W
	transform_.pos = STAGE_POS;
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	transform_.Update();
	//�R���W�����̍쐬
	transform_.MakeCollider(Collider::TYPE::STAGE);

	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//�G�t�F�N�g
	effectManager_->Add(EFFECT_TYPE,
		{ BARRIER_SCALE.x,BARRIER_SCALE.y,BARRIER_SCALE.z },
		{ 0.0f,900.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		true,
		false,
		resMng_.Load(ResourceManager::SRC::BARRIOR_DOME).handleId_);
	//�o���A�̃G�t�F�N�g�̍Đ�
	effectManager_->Play(EFFECT_TYPE);
	//�o���A�̍Đ����x��ݒ�
	effectManager_->SetSpeed(EFFECT_TYPE, BARRIER_SPEED);
}

Stage::~Stage(void)
{
}

void Stage::Draw(void)
{
	//�`��
	MV1DrawModel(transform_.modelId);
	//�G�t�F�N�g�Đ�
	effectManager_->Update();
}

Collider* Stage::GetCollider(void)
{
	return transform_.collider;
}
