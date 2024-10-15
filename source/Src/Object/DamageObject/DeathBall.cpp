#include "DeathBall.h"
#include"../Time/DeltaTime.h"

DeathBall::DeathBall(int playerType, const VECTOR& pos) :DamageObject(playerType), playerPos_(pos)
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//�傫��
	transform_.scl = { 150.0f,150.0f,150.0f };
	//���W
	transform_.pos = { 0.0f,0.0f,0.0f };
	//��]
	transform_.quaRot = Quaternion();
	//��L������Ԃɂ���
	activeFlag_ = false;
	//�_���[�W�ʂ̐ݒ�
	damage_ = 500.0f;
	//�X�V�����̏��
	update_ = std::bind(&DeathBall::WaitUpdate, this);
	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(BALL_STATE::BALL), { 6000.0f,6000.0f,6000.0f }, true, resMng_.Load(ResourceManager::SRC::DEATH_BALL).handleId_);
}

DeathBall::~DeathBall()
{

}

void DeathBall::Draw(void)
{
}

void DeathBall::Update(VECTOR enemyPos)
{
	update_();
	//�G�t�F�N�g�̍X�V
	effectManager_->Update();
	//�g�����X�t�H�[���̍X�V
	transform_.Update();
}

void DeathBall::Activate(void)
{
	//���W���v���C���[�̓���ɐݒ�
	SetPos(VECTOR{ playerPos_.x, POS_DETHBALL_Y, playerPos_.z });

	//�L��������
	activeFlag_ = true;
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�G�t�F�N�g�̍Đ�
	effectManager_->Play(static_cast<int>(BALL_STATE::BALL));
	//�A�b�v�f�[�g��NormalUpdate�ɕύX
	update_ = std::bind(&DeathBall::NormalUpdate, this);


}

void DeathBall::InActivate(void)
{
	//��L����
	activeFlag_ = false;
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�G�t�F�N�g�̍Đ����~
	effectManager_->Stop(static_cast<int>(BALL_STATE::BALL));
	//�A�b�v�f�[�g��NormalUpdate�ɕύX
	update_ = std::bind(&DeathBall::WaitUpdate, this);

}

const int& DeathBall::GetModelId(void) const
{
	return transform_.modelId;
}

void DeathBall::WaitUpdate(void)
{
}

void DeathBall::NormalUpdate(void)
{	
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�ړ�����
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	transform_.pos.y -= 500.0f * deltatime;
	//�n�ʂ̔����܂Ŗ��܂�����A��L����
	if (transform_.pos.y<=-300.0f)
	{
		InActivate();
	}
}
