#include<DxLib.h>
#include<string>
#include"../../Utility/AsoUtility.h"
#include"../Weapon/BeamRifle.h"
#include"../Time/DeltaTime.h"
#include "BeamShot.h"

#pragma region MyRegion

//�e�̉�]
constexpr VECTOR DEFAULT_ROT = { 0.0f,180.0f,0.0f };
//�e�̑傫�� 
constexpr VECTOR DEFAULT_SCALE = { 1.0f,1.0f,1.0f };
//�e�̍��W
constexpr VECTOR DEFAULT_POS = { 0.0f,0.0f,0.0f };

//�r�[���G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFECT_BEAM_OFFSET = { 0.0f,0.0f,0.0f };
//�r�[���G�t�F�N�g�̑傫��
constexpr VECTOR EFECT_BEAM_SCALE = { 1.0f,1.0f,1.0f };
//�r�[���G�t�F�N�g�̉�]
constexpr VECTOR EFECT_BEAM_ROT = { 0.0f,0.0f,0.0f };

//�q�b�g�G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFECT_HIT_OFFSET = { 0.0f,0.0f,0.0f };
//�q�b�g�G�t�F�N�g�̑傫��
constexpr VECTOR EFECT_HIT_SCALE = { 1.0f,1.0f,1.0f };
//�q�b�g�G�t�F�N�g�̉�]
constexpr VECTOR EFECT_HIT_ROT = { 0.0f,0.0f,0.0f };

//�G�̏������W
constexpr VECTOR EFECT_BEAM_POS = { 0.0f,0.0f,0.0f };
//�G�ւ̏����x�N�g��
constexpr VECTOR DEFAULT_HORMING_VEC = { 0.0f,0.0f,0.0f };

//�ʏ�_���[�W
constexpr float DAMAGE_NORMAL = 50.0f;
//��_���[�W
constexpr float DAMAGE_BIG = 200.0f;

//�����N�[���^�C��
constexpr float DEFAULT_COOLTIME = 240.0f;

//�N�[���^�C����0�ɂ���
constexpr float COOLTIME_ZERO = 0.0f;

//�r�[�������ˉ\�ɂȂ�܂ł̎���
constexpr float LIMIT_BEAM_COUNT = 320.0f;
//�r�[���̍Ĕ��ˉ\���Ԃ��v������f���^�^�C���ɂ�����萔
constexpr float BEAM_COUNT_RATE = 60.0f;

//�������Ԃ�0�ɂ���
constexpr float DEATH_TIME_ZERO = 0.0f;
//�r�[���̏��ł��v������f���^�^�C���ɂ�����萔
constexpr float DEATH_COUNT_RATE = 60.0f;
//�r�[�����A�N�e�B�u�ł����鎞��
constexpr float LIMIT_DEATH_COUNT_ = 140.0f;

//�z�[�~���O���Ă����鎞��
constexpr float LIMIT_HORMING_COUNT = 120.0f;

//�e���ǂ�������G�̍��W��������ɂ��炷���߂̕␳�l
constexpr float OFFSET_ENEMY = 100.0f;

//�e���ǂ�������{�X�̍��W��������ɂ��炷���߂̕␳�l
constexpr float OFFSET_BOSS_ENEMY = 1500.0f;


#pragma endregion

BeamShot::BeamShot(int playerType, const bool& isHorming, int num, int playMode) :DamageObject(playerType),isHorming_(isHorming)
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//�v���C���[�^�C�v�̐ݒ�
	playerType_=playerType;
	//�傫��
	transform_.scl = DEFAULT_SCALE ;
	//���W
	transform_.pos = DEFAULT_POS;
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ DEFAULT_ROT.x, AsoUtility::Deg2RadF(DEFAULT_ROT.y), DEFAULT_ROT.z });
	//��L����
	isActive_ = false;
	//�ҋ@��Ԃ̍X�V�����ɂ���
	update_ = std::bind(&BeamShot::WaitUpdate, this);
	//�G���W�̏�����
	enemyPos_ = EFECT_BEAM_POS ;
	//�z�[�~���O�x�N�g���̏�����
	hormingVec_ = DEFAULT_HORMING_VEC;
	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(BEAM_STATE::BEAM),
		EFECT_BEAM_SCALE,
		EFECT_BEAM_OFFSET,
		EFECT_BEAM_ROT,
		true,
		false,
		resMng_.Load(ResourceManager::SRC::BEAM).handleId_);

	effectManager_->Add(static_cast<int>(BEAM_STATE::HIT),
		HIT_EFFECT_SCARE,
		EFECT_HIT_OFFSET,
		EFECT_BEAM_ROT,
		false,
		false,
		resMng_.Load(ResourceManager::SRC::HIT).handleId_);
	//�ʏ�_���[�W��
	damage_ = DAMAGE_NORMAL;
	//��_���[�W��
	bigDamage_ = DAMAGE_BIG;
	//�N�[���^�C���̐ݒ�
	coolTime_ = DEFAULT_COOLTIME;
	//�������ԃJ�E���g�̏�����
	deathCount_ = DEATH_TIME_ZERO;

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
	//
	update_();
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
	isActive_ = true;
	//�r�[����L�����܂ł̎��Ԃ̐ݒ�
	deathCount_ = LIMIT_DEATH_COUNT_;
	//�r�[���G�t�F�N�g�̍Đ�
	effectManager_->Play(static_cast<int>(BEAM_STATE::BEAM),true);
	//�G�ւ̃z�[�~���O����
	//Horming();
	//�z�[�~���O��ԂȂ�
	if (isHorming_)
	{	//�z�[�~���O�A�b�v�f�[�g�ɂ���
		update_ = std::bind(&BeamShot::HormingUpdate, this);
	}
	else
	{	//�m�[�}���A�b�v�f�[�g�ɂ���
		update_ = std::bind(&BeamShot::NormalUpdate, this);
	}
	transform_.Update();	
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);

}

void BeamShot::InActivate(void)
{
	//��L����
	isActive_ = false;
	//�e�̐����\���Ԃ����Z�b�g
	deathCount_ = DEATH_TIME_ZERO;
	//�r�[���G�t�F�N�g�̍Đ����~�߂�
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));
	//�z�[�~���O���Ԃ̃��Z�b�g
	hormingCount_ = COOLTIME_ZERO;
	//�ҋ@��ԂɕύX
	update_ = std::bind(&BeamShot::WaitUpdate, this);
}

void BeamShot::Hit(void)
{
	//��L����
	isActive_ = false;
	//�e�̐����\���Ԃ����Z�b�g
	deathCount_ = DEATH_TIME_ZERO;
	//�q�b�g�G�t�F�N�g�̍Đ�
	effectManager_->Play(static_cast<int>(BEAM_STATE::HIT));
	//�r�[���G�t�F�N�g�̍Đ����~�߂�
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));
	//
	update_ = std::bind(&BeamShot::WaitUpdate, this);
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
	//���̃A�b�v�f�[�g�ł͉������Ȃ�
}

void BeamShot::NormalUpdate(void)
{
	//�ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * deltaTime_));
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�����\���Ԍv��
	TimeToDeath();
}

void BeamShot::HormingUpdate(void)
{	
	//�z�[�~���O����
	Horming();
	//�ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * deltaTime_ ));
	//�����蔻��̍X�V
	MV1RefreshCollInfo(transform_.modelId);
	//�����\���Ԍv��
	TimeToDeath();
	//�z�[�~���O�\���Ԍv��
	hormingCount_ += deltaTime_ * DEATH_COUNT_RATE;
	//�z�[�~���O�\���Ԃ𒴂�����
	if (hormingCount_> LIMIT_HORMING_COUNT)
	{
		//�z�[�~���O�����̂Ȃ��̃A�b�v�f�[�g�����ɕύX
		update_ = std::bind(&BeamShot::NormalUpdate, this);
	}
}

void BeamShot::TimeToDeath(void)
{
	//�����\���Ԍv��
	deathCount_ -= deltaTime_ * DEATH_COUNT_RATE;
	//�����\���Ԃ�0�ɂȂ�����
	if (deathCount_ < DEATH_TIME_ZERO )
	{
		deathCount_ = DEATH_TIME_ZERO;
		//��L����
		InActivate();
	}
}

void BeamShot::Horming(void)
{
	//�G���W�̍X�V
	VECTOR enemyPos = { enemyPos_.x,enemyPos_.y + offsetEnemy_,enemyPos_.z };
	//�x�N�g�������A���K��
	hormingVec_ = VNorm(VSub(enemyPos, transform_.pos));
	//���̃x�N�g���̌����ɕ�����ς���
	transform_.quaRot = Quaternion::LookRotation(hormingVec_);
}

void BeamShot::CoolTimeCount(void)
{
	//�N�[���^�C�����K��𒴂��ĂȂ���Ή��Z����
	if (!(coolTime_> LIMIT_BEAM_COUNT))
	{
		coolTime_ += deltaTime_* BEAM_COUNT_RATE;
		//���𒴂�����ő�̒l�ɌŒ肷��
		if (coolTime_ > LIMIT_BEAM_COUNT)
		{
			coolTime_ = LIMIT_BEAM_COUNT;
		}
	}
	//�N�[���^�C�����K��𒴂��Ă���΍ő�̒l�ɌŒ肷��
	else
	{
		coolTime_ = LIMIT_BEAM_COUNT;
	}
}

void BeamShot::InActivateHorming()
{
	update_ = std::bind(&BeamShot::NormalUpdate, this);

}
