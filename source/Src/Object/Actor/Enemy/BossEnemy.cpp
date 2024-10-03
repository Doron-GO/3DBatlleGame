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
static constexpr VECTOR SCALE_BOSS_ENEMY = { 10.0f,10.0f,10.0f };

//���W
static constexpr VECTOR DEFAULT_POS = { 0.0f, -30.0f, 1600.0f };

//�{�X�^�C�v
static constexpr int ENEMY_TYPE = 1;

//�{�XHP
static constexpr float ENEMY_HP = 500.0f;

//�����蔻��J�v�Z���̔��a
static constexpr float CAPSUL_RADIUS = 300.0f;

//�J�v�Z������W
static constexpr float CAPSUL_TOP = 2000.0f;

//�J�v�Z�������W
static constexpr float CAPSUL_DOWN = 200.0f;

//�U���G�t�F�N�g�̑傫��
static constexpr VECTOR EFFECT_EXPLOSION_SCALE = { 200.0f,200.0f,200.0f };

#pragma endregion

BossEnemy::BossEnemy()
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//�傫��
	transform_.scl = SCALE_BOSS_ENEMY;
	//���W
	transform_.pos = { DEFAULT_POS.x, -DEFAULT_POS.x, (DEFAULT_POS.z ) };
	//��]
	transform_.quaRot = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//�A�j���[�V�����R���g���[���[�̐���
	roboAnimeController_ = std::make_unique <RobotAnimeController>(transform_.modelId);

	//�J�v�Z�������炢����N���X�̐���
	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSUL_TOP, CAPSUL_DOWN, CAPSUL_RADIUS);
	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_= std::make_unique<EffectManager>(transform_);
	//�A�j���[�V�����̓ǂݍ���
	InitAnimation();

	//�ҋ@��Ԃɐݒ�@
	ChangeState(std::make_unique<WaitState>(*this));

	//HP��ݒ�
	playerHp_ = ENEMY_HP;

}

BossEnemy::~BossEnemy()
{
	
}


void BossEnemy::InitAnimation(void)
{
	//�A�j���[�V�����̒ǉ�
	roboAnimeController_->Add(static_cast<int>(STATE::SHOOTING), PATH_ANIMATION_BOSS_ENEMY + "Boss_Shooting.mv1", 10.0f, 750.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::WAIT), PATH_ANIMATION_BOSS_ENEMY + "Boss_Idle.mv1", 60.0f, 427.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::DEAD), PATH_ANIMATION_BOSS_ENEMY + "Dying.mv1", 30.0f, 174.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::DOWN), PATH_ANIMATION_BOSS_ENEMY + "Crouch.mv1", 100.0f, 427.0f);
	roboAnimeController_->Update();
	effectManager_->Add(static_cast<int>(STATE::DEAD), EFFECT_EXPLOSION_SCALE, false,resMng_.Load(ResourceManager::SRC::EXPLOSION).handleId_);

}

void BossEnemy::SetEnemyPosition(const VECTOR* enemyPos)
{
	playerPos_ = enemyPos;
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
	spMoves_.emplace(SP_MOVE::DEATH_BALL, std::make_unique <DeathBall>(ENEMY_TYPE,*playerPos_));

}

void BossEnemy::ChangeDeathState(void)
{
	ChangeState(std::make_unique<DeadState>(*this));
	return;
}

void BossEnemy::PlayEffect(STATE state)
{
	effectManager_->Play(static_cast<int>(state));
}

void BossEnemy::Update()
{
	//�f���^�^�C���̍X�V
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	//�X�e�[�g�̃A�b�v�f�[�g
	state_->Update();
	//�U���I�u�W�F�N�g�̃A�b�v�f�[�g
	for(auto& spMove: spMoves_)
	{
		spMove.second->Update(*playerPos_);
	}
	//�A�j���[�V�����R���g���[���[�̃A�b�v�f�[�g
	roboAnimeController_->Update();
	//�����蔻��̍X�V
	capsule_->Update();
	//�G�t�F�N�g�}�l�[�W���[�̍X�V
	effectManager_->Update();
	DrawFormatStringF(0.0f, 880.0f, 0xffffff, "playerHp_%f", playerHp_);

}

void BossEnemy::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	for (auto& spMove : spMoves_)
	{
		spMove.second->Draw();
	}
}

void BossEnemy::SetPlayerPosition(const VECTOR* playerPos)
{
	playerPos_ = playerPos;
}

const VECTOR& BossEnemy::GetBossEnemyPos(void) const
{
	return transform_.pos;
}

void BossEnemy::ChangeState(std::unique_ptr<StateBase> state)
{
	state_ = std::move(state);
}

