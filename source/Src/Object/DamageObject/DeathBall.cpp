#include "DeathBall.h"
#include"../Time/DeltaTime.h"

//�o������Y���W
constexpr float POS_DETHBALL_Y = 5500.0f;

constexpr float DAMAGE = 500.0f;

constexpr float BALL_SPEED = 500.0f;

constexpr float LIMIT_POS_Y = -300.0f;

//Ball�̑傫��
constexpr VECTOR BALL_SCALE = { 150.0f,150.0f,150.0f };

//Ball�̍��W
constexpr VECTOR BALL_POS = { 0.0f,0.0f,0.0f };


//�G�t�F�N�g�̑傫��
constexpr VECTOR EFFECT_BALL_SCALE = { 6000.0f,6000.0f,6000.0f };

//�G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFFECT_BALL_OFFSET = { 1.0f,1.0f,1.0f };

//�G�t�F�N�g�̉�]
constexpr VECTOR EFFECT_BALL_ROT = { 1.0f,1.0f,1.0f };


DeathBall::DeathBall(int playerType, const VECTOR& pos) :DamageObject(playerType), playerPos_(pos)
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//�傫��
	transform_.scl = BALL_SCALE;
	//���W
	transform_.pos = BALL_POS;
	//��]
	transform_.quaRot = Quaternion();
	//��L������Ԃɂ���
	isActive_ = false;
	//�_���[�W�ʂ̐ݒ�
	damage_ = DAMAGE;
	//�X�V�����̏��
	update_ = std::bind(&DeathBall::WaitUpdate, this);
	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(BALL_STATE::BALL),
		EFFECT_BALL_SCALE,
		EFFECT_BALL_OFFSET,
		EFFECT_BALL_ROT,
		true,
		false,
		resMng_.Load(ResourceManager::SRC::DEATH_BALL).handleId_);
}

DeathBall::~DeathBall()
{

}

void DeathBall::Draw(void)
{
}

void DeathBall::Update(VECTOR enemyPos)
{
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();

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
	isActive_ = true;
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
	isActive_ = false;
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
	//�������Ȃ�
}

void DeathBall::NormalUpdate(void)
{	
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�ړ�����
	transform_.pos.y -= BALL_SPEED * deltaTime_;
	//�n�ʂ̔����܂Ŗ��܂�����A��L����
	if (transform_.pos.y<= LIMIT_POS_Y)
	{
		InActivate();
	}
}
