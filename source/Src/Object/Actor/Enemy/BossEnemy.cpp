#include"../../State/BossEnemyState/WaitState.h"
#include"../../State/BossEnemyState/DeadState.h"
#include"../../../Manager/ResourceManager.h"
#include"../../../Manager/EffectManager.h"
#include"../../../Utility/AsoUtility.h"
#include"../../../Manager/RobotAnimeController.h"
#include"../../../../Config.h"
#include"../../DamageObject/DeathBall.h"
#include"../../Time/DeltaTime.h"
#include "BossEnemy.h"
#include "BossEnemy.h"
#include<DxLib.h>
#pragma region Parameter

//�傫��
constexpr VECTOR SCALE_BOSS_ENEMY = { 10.0f,10.0f,10.0f };

//���W
constexpr VECTOR DEFAULT_BOSS_POS = { 0.0f, -500.0f, 1600.0f };

//�{�X�^�C�v
constexpr int ENEMY_TYPE = 1;

//�{�XHP
constexpr float ENEMY_HP = 100.0f;

//�����蔻��J�v�Z���̔��a
constexpr float CAPSUL_RADIUS = 300.0f;

//�J�v�Z������W
constexpr float CAPSUL_TOP = 2000.0f;

//�J�v�Z�������W
constexpr float CAPSUL_DOWN = 200.0f;

//���S���G�t�F�N�g�̑傫��
constexpr VECTOR EFFECT_EXPLOSION_SCALE = { 200.0f,200.0f,200.0f };

//�U���G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFFECT_EXPLOSION_OFFSET = { 0.0f,0.0f,0.0f };

//�U���G�t�F�N�g�̉�]
constexpr VECTOR EFFECT_EXPLOSION_ROT = { 0.0f,0.0f,0.0f };

//�V���[�g�A�j���[�V�����̍Đ����x
constexpr float ANIM_SHOOT_SPEED = 10.0f;
//�V���[�g�A�j���[�V�����̑��Đ�����
constexpr float  TOTAL_TIME_ANIM_SHOOT = 750.0f;

//�A�C�h���A�j���[�V�����̍Đ����x
constexpr float ANIM_IDLE_SPEED = 60.0f;
//�A�C�h���A�j���[�V�����̑��Đ�����
constexpr float  TOTAL_TIME_ANIM_IDLE = 427.0f;

//���S�A�j���[�V�����̍Đ����x
constexpr float ANIM_DEAD_SPEED = 30.0f;
//���S�A�j���[�V�����̑��Đ�����
constexpr float  TOTAL_TIME_ANIM_DEAD = 174.0f;

//�_�E���A�j���[�V�����̍Đ����x
constexpr float ANIM_DOWN_SPEED = 100.0f;
//�_�E���A�j���[�V�����̑��Đ�����
constexpr float  TOTAL_TIME_ANIM_DOWN = 427.0f;



#pragma endregion

BossEnemy::BossEnemy(int playerType)
{

	actorType_ = static_cast<ACTOR_TYPE>(playerType);

	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//�傫��
	transform_.scl = SCALE_BOSS_ENEMY;
	//���W
	transform_.pos = { DEFAULT_BOSS_POS.x, DEFAULT_BOSS_POS.y, (DEFAULT_BOSS_POS.z) };
	//��]
	transform_.quaRot = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//�A�j���[�V�����R���g���[���[�̐���
	robotAnimeController_ = std::make_unique <RobotAnimeController>(transform_.modelId);

	//�J�v�Z�������炢����N���X�̐���
	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSUL_TOP, CAPSUL_DOWN, CAPSUL_RADIUS);
	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_= std::make_unique<EffectManager>(transform_);
	//�A�j���[�V�����̓ǂݍ���
	InitAnimation();

	//�ҋ@��Ԃɐݒ�@
	ChangeState(std::make_unique<WaitState>(*this));

	//HP��ݒ�
	hp_ = ENEMY_HP;

}

BossEnemy::~BossEnemy(void)
{
	
}


void BossEnemy::InitAnimation(void)
{
	//�A�j���[�V�����̒ǉ�
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::SHOOTING),
				PATH_ANIMATION_BOSS_ENEMY + "Boss_Shooting.mv1",
				ANIM_SHOOT_SPEED,
				TOTAL_TIME_ANIM_SHOOT);
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::WAIT),
				PATH_ANIMATION_BOSS_ENEMY + "Boss_Idle.mv1",
				ANIM_IDLE_SPEED,
				TOTAL_TIME_ANIM_IDLE);
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::DEAD),
				PATH_ANIMATION_BOSS_ENEMY + "Dying.mv1",
				ANIM_DEAD_SPEED,
				TOTAL_TIME_ANIM_DEAD);
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::DOWN),
				PATH_ANIMATION_BOSS_ENEMY + "Crouch.mv1",
				ANIM_DOWN_SPEED,
				TOTAL_TIME_ANIM_DOWN);
	robotAnimeController_->Update();

	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(BOSS_STATE::DEAD),
		EFFECT_EXPLOSION_SCALE,
		EFFECT_EXPLOSION_OFFSET,
		EFFECT_EXPLOSION_ROT,
		false,
		false,
		resMng_.Load(ResourceManager::SRC::EXPLOSION).handleId_);

}

void BossEnemy::ActiveSpMove(SP_MOVE spMove)
{
	spMoves_[spMove]->Activate();
}

const bool& BossEnemy::IsActiveSpMove(SP_MOVE spMove) 
{
	//�U�����L�������ǂ�����Ԃ�
	return 	spMoves_[spMove]->IsActive();
}

const std::map<BossEnemy::SP_MOVE, std::unique_ptr<DamageObject>>& BossEnemy::GetSpMove() const
{
	return spMoves_;
}

void BossEnemy::MakeSpMoveObjects(void)
{
	spMoves_.emplace(SP_MOVE::DEATH_BALL, std::make_unique <DeathBall>(ENEMY_TYPE,*enemyPos_));

}

void BossEnemy::ChangeDeathState(void)
{
	ChangeState(std::make_unique<DeadState>(*this));
	return;
}

void BossEnemy::PlayEffect(BOSS_STATE state)
{
	effectManager_->Play(static_cast<int>(state));
}

const BossEnemy::STATE& BossEnemy::GetState(void)
{
	return actorState_;
}

void BossEnemy::Update(void)
{
	//�f���^�^�C���̍X�V
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	//�X�e�[�g�̃A�b�v�f�[�g
	state_->Update();
	//�U���I�u�W�F�N�g�̃A�b�v�f�[�g
	for(auto& spMove: spMoves_)
	{
		spMove.second->Update(*enemyPos_);
	}
	//�A�j���[�V�����R���g���[���[�̃A�b�v�f�[�g
	robotAnimeController_->Update();
	//�����蔻��̍X�V
	capsule_->Update();
	//�G�t�F�N�g�}�l�[�W���[�̍X�V
	effectManager_->Update();

}

void BossEnemy::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	for (auto& spMove : spMoves_)
	{
		spMove.second->Draw();
	}
}

const VECTOR& BossEnemy::GetBossEnemyPos(void) const
{
	return transform_.pos;
}

void BossEnemy::ChangeState(std::unique_ptr<StateBase> state)
{
	state_ = std::move(state);
}

