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

BossEnemy::BossEnemy()
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	transform_.scl = SCALE_BOSS_ENEMY;
	transform_.pos = { DEFAULT_POS.x, -DEFAULT_POS.x, (DEFAULT_POS.z ) };
	transform_.quaRot = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });	
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	roboAnimeController_ = std::make_unique <RobotAnimeController>(transform_.modelId);

	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSUL_TOP, CAPSUL_DOWN, CAPSUL_RADIUS);
	effectManager_= std::make_unique<EffectManager>(transform_);
	InitAnimation();

	ChangeState(std::make_unique<WaitState>(*this));

	playerHp_ = ENEMY_HP;

}

BossEnemy::~BossEnemy()
{
	
}

void BossEnemy::Init()
{

}

void BossEnemy::InitAnimation(void)
{
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

const VECTOR& BossEnemy::GetEnemyPosition(void) const
{
	return *playerPos_;
}

void BossEnemy::ActiveSpMove(SP_MOVE spMove)
{
	spMoves_[spMove]->Activate();
}

void BossEnemy::InActiveSpMove(SP_MOVE spMove)
{
	spMoves_[spMove]->InActivate();
}

const bool& BossEnemy::IsActiveSpMove(SP_MOVE spMove) 
{
	return 	spMoves_[spMove]->IsActive();
}

const int& BossEnemy::GetSpMoveModelId( SP_MOVE spMove) 
{	
	return spMoves_[spMove]->GetModelId();
}

const std::map<BossEnemy::SP_MOVE, std::unique_ptr<DamageObject>>& BossEnemy::GetSpMove() const
{
	return spMoves_;
}

void BossEnemy::MakeObjects(void)
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
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	state_->Update();
	for(auto& spMove: spMoves_)
	{
		spMove.second->Update(*playerPos_);
	}
	roboAnimeController_->Update();
	capsule_->Update();
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

