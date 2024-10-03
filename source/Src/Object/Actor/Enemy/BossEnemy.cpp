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

//大きさ
static constexpr VECTOR SCALE_BOSS_ENEMY = { 10.0f,10.0f,10.0f };

//座標
static constexpr VECTOR DEFAULT_POS = { 0.0f, -30.0f, 1600.0f };

//ボスタイプ
static constexpr int ENEMY_TYPE = 1;

//ボスHP
static constexpr float ENEMY_HP = 500.0f;

//当たり判定カプセルの半径
static constexpr float CAPSUL_RADIUS = 300.0f;

//カプセル上座標
static constexpr float CAPSUL_TOP = 2000.0f;

//カプセル下座標
static constexpr float CAPSUL_DOWN = 200.0f;

//攻撃エフェクトの大きさ
static constexpr VECTOR EFFECT_EXPLOSION_SCALE = { 200.0f,200.0f,200.0f };

#pragma endregion

BossEnemy::BossEnemy()
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//大きさ
	transform_.scl = SCALE_BOSS_ENEMY;
	//座標
	transform_.pos = { DEFAULT_POS.x, -DEFAULT_POS.x, (DEFAULT_POS.z ) };
	//回転
	transform_.quaRot = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//アニメーションコントローラーの生成
	roboAnimeController_ = std::make_unique <RobotAnimeController>(transform_.modelId);

	//カプセルあたらい判定クラスの生成
	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSUL_TOP, CAPSUL_DOWN, CAPSUL_RADIUS);
	//エフェクトマネージャーの生成
	effectManager_= std::make_unique<EffectManager>(transform_);
	//アニメーションの読み込み
	InitAnimation();

	//待機状態に設定　
	ChangeState(std::make_unique<WaitState>(*this));

	//HPを設定
	playerHp_ = ENEMY_HP;

}

BossEnemy::~BossEnemy()
{
	
}


void BossEnemy::InitAnimation(void)
{
	//アニメーションの追加
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
	//攻撃が有効中かどうかを返す
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
	//デルタタイムの更新
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	//ステートのアップデート
	state_->Update();
	//攻撃オブジェクトのアップデート
	for(auto& spMove: spMoves_)
	{
		spMove.second->Update(*playerPos_);
	}
	//アニメーションコントローラーのアップデート
	roboAnimeController_->Update();
	//当たり判定の更新
	capsule_->Update();
	//エフェクトマネージャーの更新
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

