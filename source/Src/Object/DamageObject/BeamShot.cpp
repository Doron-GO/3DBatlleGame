#include<DxLib.h>
#include<string>
#include"../../Utility/AsoUtility.h"
#include"../Weapon/BeamRifle.h"
#include"../Time/DeltaTime.h"
#include "BeamShot.h"


BeamShot::BeamShot(int playerType, const bool& isHorming, int num, int playMode) :DamageObject(playerType),isHorming_(isHorming)
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//�v���C���[�^�C�v�̐ݒ�
	playerType_=playerType;
	//�傫��
	transform_.scl = { 1.0f,1.0f,1.0f };
	//���W
	transform_.pos = { 0.0f,0.0f,0.0f };
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//��L����
	activeFlag_ = false;
	//�ҋ@��Ԃ̍X�V�����ɂ���
	_update = &BeamShot::WaitUpdate;
	//�G���W�̏�����
	enemyPos_ = { 0.0f,0.0f,0.0f };
	//�z�[�~���O�x�N�g���̏�����
	hormingVec_ = { 0.0f,0.0f,0.0f };
	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(BEAM_STATE::BEAM), transform_.scl,true,resMng_.Load(ResourceManager::SRC::BEAM).handleId_);
	effectManager_->Add(static_cast<int>(BEAM_STATE::HIT), HIT_EFFECT_SCARE,false,resMng_.Load(ResourceManager::SRC::HIT).handleId_);
	//�ʏ�_���[�W��
	damage_ = 50.0f;
	//��_���[�W��
	bigDamage_ = 200.0f;
	//�N�[���^�C���̐ݒ�
	coolTime_ = 240.0f;

	//���[�h�ɂ���ăI�t�Z�b�g�l��ς���
	if (static_cast<PLAY_MODE>(playMode) == PLAY_MODE::SINGLE_MODE)
	{
		offsetEnemy_ = OFFSET_BOSS_ENEMY;
	}
	else
	{
		offsetEnemy_ = OFFSET_ENEMY;
	}
}

void BeamShot::Update(VECTOR enemyPos)
{
	//�f���^�^�C���̍X�V
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	//�G�̍��W�̍X�V
	enemyPos_ = enemyPos;
	(this->*_update)();
	//�G�t�F�N�g�}�l�[�W���[�̍X�V
	effectManager_->Update();
	//�g�����X�t�H�[���̍X�V
	transform_.Update();
	//�N�[���^�C���̃J�E���g�X�V
	CoolTimeCount();
}

void BeamShot::Draw(void)
{
}

void BeamShot::Activate(void)
{
	//�L����
	activeFlag_ = true;
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�r�[����L�����܂ł̎��Ԃ̐ݒ�
	deathCount_ = DEATH_COUNT_TIME;
	//�r�[���G�t�F�N�g�̍Đ�
	effectManager_->Play(static_cast<int>(BEAM_STATE::BEAM));
	//�G�ւ̃z�[�~���O����
	Horming();
	//�z�[�~���O��ԂȂ�
	if (isHorming_)
	{	//�z�[�~���O�A�b�v�f�[�g�ɂ���
		_update = &BeamShot::HormingUpdate;
	}
	else
	{	//�m�[�}���A�b�v�f�[�g�ɂ���
		_update = &BeamShot::NormalUpdate;
	}
	transform_.Update();
}

void BeamShot::InActivate(void)
{
	//�L����
	activeFlag_ = false;
	//�r�[���G�t�F�N�g�̍Đ����~�߂�
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));
	//�z�[�~���O���Ԃ̃��Z�b�g
	hormingCount_ = 0.0f;
	//�ҋ@��ԂɕύX
	_update = &BeamShot::WaitUpdate;
}

void BeamShot::Hit(void)
{
	//��L����
	activeFlag_ = false;
	//�e�̐����\���Ԃ����Z�b�g
	deathCount_ = 0.0f;
	//�q�b�g�G�t�F�N�g�̍Đ�
	effectManager_->Play(static_cast<int>(BEAM_STATE::HIT));
	//�r�[���G�t�F�N�g�̍Đ����~�߂�
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));

	_update = &BeamShot::WaitUpdate;
}

const int& BeamShot::GetModelId(void) const
{
	return transform_.modelId;
}

const float& BeamShot::GetBigDamage(void) const
{
	return bigDamage_;
}

void BeamShot::WaitUpdate(void)
{
}

void BeamShot::NormalUpdate(void)
{
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	//�ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * (deltatime * 100.0f)));
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�����\���Ԍv��
	TimeToDeath();
}

void BeamShot::HormingUpdate(void)
{	
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	//�z�[�~���O����
	Horming();
	//�ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * (deltatime * 100.0f)));
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�����\���Ԍv��
	TimeToDeath();
	//�z�[�~���O�\���Ԍv��
	hormingCount_ += deltatime * DEATH_COUNT_RATE;
	//�z�[�~���O�\���Ԃ𒴂�����
	if (hormingCount_> HORMING_COUNT_TIME)
	{
		//�z�[�~���O�����̂Ȃ��̃A�b�v�f�[�g�����ɕύX
		_update = &BeamShot::NormalUpdate;
	}
}

void BeamShot::TimeToDeath(void)
{
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	//�����\���Ԍv��
	deathCount_ -= deltatime* DEATH_COUNT_RATE;
	//�����\���Ԃ������Ă�����
	if (deathCount_<0)
	{
		deathCount_ = 0;
		//��L����
		InActivate();
	}
}

void BeamShot::Horming(void)
{
	VECTOR enemyPos = { enemyPos_.x,enemyPos_.y + offsetEnemy_,enemyPos_.z };
	//�x�N�g�������A���K��
	hormingVec_ = VNorm(VSub(enemyPos, transform_.pos));
	//���̃x�N�g���̌����ɕ�����ς���
	transform_.quaRot = Quaternion::LookRotation(hormingVec_);
}

void BeamShot::CoolTimeCount(void)
{
	//�N�[���^�C�����K��𒴂��ĂȂ���Ή��Z����
	if (!(coolTime_> BEAM_COUNT_TIME))
	{
		coolTime_ += deltaTime_* BEAM_COUNT_RATE;
		//���𒴂�����ő�̒l�ɌŒ肷��
		if (coolTime_ > BEAM_COUNT_TIME)
		{
			coolTime_ = BEAM_COUNT_TIME;
		}
	}
	//�N�[���^�C�����K��𒴂��Ă���΍ő�̒l�ɌŒ肷��
	else
	{
		coolTime_ = BEAM_COUNT_TIME;
	}
}

void BeamShot::InActivateHorming()
{
	_update = &BeamShot::NormalUpdate;
}
